#include "Engine/Render/OpenGl/Renderer.hpp"

#include <SDL_video.h>

#include <entt/entity/registry.hpp>

#include "Engine/Core/Window.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/OpenGl/Model/RenderBatch.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"
#include "Engine/Render/OpenGl/WindowContext.hpp"

#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"

#include "Engine/Render/OpenGl/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/OpenGl/ECS/RenderBatcher.hpp"
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

    AssertD(program == nullptr);
    program = std::make_unique<ShaderProgram>("BasicShader");
    program->SetUp();
}

void SHV::OpenGl::Renderer::TearDown() {
    AssertD(program != nullptr);
    program->TearDown();
    program = nullptr;
}

void SHV::OpenGl::Renderer::SetUpScene(Scene& scene) {
    std::unique_ptr<SHV::RenderBatcher> renderBatcher =
        std::make_unique<SHV::OpenGl::RenderBatcher>();
    scene.AddSystem<SHV::RenderSystem<SHV::OpenGl::RenderComponent>>(
        std::move(renderBatcher));
}

void SHV::OpenGl::Renderer::TearDownScene(Scene& scene) {
    scene.RemoveSystem<SHV::RenderSystem<SHV::OpenGl::RenderComponent>>();
}

void SHV::OpenGl::Renderer::Draw(const Scene& scene) {
    const auto renderView = scene.GetRegistry().view<SHV::RenderComponent>();

    auto cameraEntity = scene.GetEntityWithActiveCamera();
    AssertE(cameraEntity != entt::null);

    const auto& cameraComponent =
        scene.GetRegistry().get<SHV::CameraComponent>(cameraEntity);
    const auto& cameraTransform =
        scene.GetRegistry().get<SHV::TransformComponent>(cameraEntity);

    for (const auto& [entity, renderComponent] : renderView.each()) {
        const auto& openGlRenderComponent =
            scene.GetRegistry().try_get<SHV::OpenGl::RenderComponent>(entity);
        AssertE(openGlRenderComponent != nullptr);

        GLuint projectionUniformLocation =
            program->GetUniformLocation("projection");
        GLuint viewUniformLocation = program->GetUniformLocation("view");
        GLuint modelUniformLocation = program->GetUniformLocation("model");

        const TransformComponent* transformComponent =
            Entity::GetFirstComponentInHierarchy<TransformComponent>(
                scene.GetRegistry(), entity);
        AssertE(transformComponent != nullptr);

        const auto& renderBatch = openGlRenderComponent->renderBatch;

        program->Use();

        glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE,
                           glm::value_ptr(cameraComponent.projection));
        glUniformMatrix4fv(
            viewUniformLocation, 1, GL_FALSE,
            glm::value_ptr(transformComponent->GetLocalMatrix()));
        glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE,
                           glm::value_ptr(cameraTransform.GetLocalMatrix()));

        renderBatch.Bind();
        glDrawElements(GL_TRIANGLES, renderBatch.GetIndexCount(),
                       GL_UNSIGNED_INT, 0);
        renderBatch.Unbind();
    }
}

void SHV::OpenGl::Renderer::BeginFrame() {
    const auto viewportSize = windowContext.GetViewportSize();
    glViewport(0, 0, viewportSize.x, viewportSize.y);

    glClearColor(windowContext.GetWindow().GetWindowConfig().clearColor.r,
                 windowContext.GetWindow().GetWindowConfig().clearColor.g,
                 windowContext.GetWindow().GetWindowConfig().clearColor.b,
                 windowContext.GetWindow().GetWindowConfig().clearColor.a);

    glClear(GL_COLOR_BUFFER_BIT);
};

void SHV::OpenGl::Renderer::EndFrame() {
    SDL_GL_SwapWindow(&windowContext.GetWindow().GetWindow());
};

std::optional<std::string> SHV::OpenGl::Renderer::ValidateExtensions() {
    return std::nullopt;

    //    This is the place for validating extensions like this:
    //    if (!SDL_GL_ExtensionSupported("GL_ARB_enhanced_layouts")) {
    //        return "GL_ARB_enhanced_layouts";
    //    }
}