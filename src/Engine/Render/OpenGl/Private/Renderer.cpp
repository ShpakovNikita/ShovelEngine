#include "Engine/Render/OpenGl/Renderer.hpp"

#include <SDL_video.h>

#include <entt/entity/registry.hpp>

#include <Tracy.hpp>

#include "Engine/Core/Window.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Common/ProfilerSystems.hpp"

#include "Engine/Render/OpenGl/Model/RenderBatch.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"
#include "Engine/Render/OpenGl/WindowContext.hpp"

#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"

#include "Engine/Render/OpenGl/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/OpenGl/ECS/RenderBatcher.hpp"
#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Render/OpenGl/Model/GPUTextureUtils.hpp"
#include "Engine/Render/OpenGl/ShaderCache.hpp"
#include "Engine/Render/ECS/Systems/RenderSystem.hpp"

SHV::OpenGl::Renderer::Renderer(SHV::OpenGl::WindowContext& aWindow)
    : SHV::Renderer(), windowContext(aWindow){};

SHV::OpenGl::Renderer::~Renderer() = default;

void SHV::OpenGl::Renderer::SetUp() {
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        throw SHV::Exception("Error initializing GLEW! %s\n",
                             glewGetErrorString(glewError));
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        throw SHV::Exception("Unable to set VSync! SDL Error: %s\n",
                             SDL_GetError());
    }

    auto nonValidExtension = ValidateExtensions();
    if (nonValidExtension) {
        throw SHV::Exception("Unable to validate extensions! %s",
                             nonValidExtension.value().c_str());
    }
}

void SHV::OpenGl::Renderer::TearDown() {}

void SHV::OpenGl::Renderer::SetUpScene(Scene& scene) const {
    std::unique_ptr<SHV::RenderBatcher> renderBatcher =
        std::make_unique<SHV::OpenGl::RenderBatcher>();
    scene.AddSystem<SHV::RenderSystem<SHV::OpenGl::RenderComponent>>(
        std::move(renderBatcher));
}

void SHV::OpenGl::Renderer::TearDownScene(Scene& scene) const {
    scene.RemoveSystem<SHV::RenderSystem<SHV::OpenGl::RenderComponent>>();
}

void SHV::OpenGl::Renderer::WaitForFrameExecutionFinish() {
    ZoneNamedN(
        __tracy, "OpenGl Render WaitForFrameExecutionFinish",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));
    glFinish();
}

void SHV::OpenGl::Renderer::Draw(const Scene& scene) {
    ZoneNamedN(
        __tracy, "OpenGl Render Draw",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    auto cameraEntity = scene.GetEntityWithActiveCamera();
    AssertE(cameraEntity != entt::null);

    const auto& cameraComponent =
        scene.GetRegistry().get<SHV::CameraComponent>(cameraEntity);
    const auto& cameraTransform =
        scene.GetRegistry().get<SHV::TransformComponent>(cameraEntity);

    const std::vector<entt::entity> renderQueue = SortedRenderQueue(scene);
    for (const auto& entity: renderQueue) {
        const auto& openGlRenderComponent =
            scene.GetRegistry().try_get<SHV::OpenGl::RenderComponent>(entity);
        AssertE(openGlRenderComponent != nullptr &&
                openGlRenderComponent->renderMaterial != nullptr);

        ShaderProgram& program =
            openGlRenderComponent->renderMaterial->GetShaderProgram();

        const TransformComponent* transformComponent =
            Entity::GetFirstComponentInHierarchy<TransformComponent>(
                scene.GetRegistry(), entity);
        AssertE(transformComponent != nullptr);

        const auto& renderBatch = openGlRenderComponent->renderBatch;

        program.Use();

        program.SetMat4("projection", cameraComponent.projection);
        program.SetMat4("view", cameraComponent.GetViewMatrix(cameraTransform.translation));
        program.SetMat4("model", transformComponent->GetWorldMatrix());
        program.SetMat4("viewRotation", cameraComponent.GetViewMatrix({}));

        renderBatch.Bind();
        openGlRenderComponent->renderMaterial->Bind();

        glDrawElements(GL_TRIANGLES,
                       static_cast<GLsizei>(renderBatch.GetIndexCount()),
                       GL_UNSIGNED_INT, 0);

        openGlRenderComponent->renderMaterial->Unbind();
        renderBatch.Unbind();
    }
}

std::shared_ptr<SHV::Texture> SHV::OpenGl::Renderer::Draw(const SHV::Scene& scene,
                                                         const SHV::Texture& renderTargetPrototype) {
    ZoneNamedN(
        __tracy, "OpenGL Render Draw to Render Target",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    const GlTextureFormat textureFormat = GPUTextureUtils::GetGlTextureFormat(renderTargetPrototype.GetTextureFormat());

    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat.internalFormat, renderTargetPrototype.GetWidth(), renderTargetPrototype.GetHeight(), 0, textureFormat.format, textureFormat.type, NULL);
    GPUTextureUtils::SetupTextureSampler(renderTargetPrototype);

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw SHV::Exception("Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, renderTargetPrototype.GetWidth(), renderTargetPrototype.GetHeight());
    glClear(GL_COLOR_BUFFER_BIT);

    Draw(scene);

    std::shared_ptr<SHV::Texture> destinationTexture = GPUTextureUtils::MakeTextureFromFrameBuffer(fbo, renderTargetPrototype);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texture);

    return destinationTexture;
}

void SHV::OpenGl::Renderer::BeginFrame() {
    ZoneNamedN(
        __tracy, "OpenGl Render BeginFrame",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    const auto viewportSize = windowContext.GetViewportSize();
    glViewport(0, 0, viewportSize.x, viewportSize.y);

    glClearColor(windowContext.GetWindow().GetWindowConfig().clearColor.r,
                 windowContext.GetWindow().GetWindowConfig().clearColor.g,
                 windowContext.GetWindow().GetWindowConfig().clearColor.b,
                 windowContext.GetWindow().GetWindowConfig().clearColor.a);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void SHV::OpenGl::Renderer::EndFrame() {
    ZoneNamedN(
        __tracy, "OpenGl Render EndFrame",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    SDL_GL_SwapWindow(&windowContext.GetWindow().GetWindow());
};

std::optional<std::string> SHV::OpenGl::Renderer::ValidateExtensions() {
    return std::nullopt;

    //    This is the place for validating extensions like this:
    //    if (!SDL_GL_ExtensionSupported("GL_ARB_enhanced_layouts")) {
    //        return "GL_ARB_enhanced_layouts";
    //    }
}
