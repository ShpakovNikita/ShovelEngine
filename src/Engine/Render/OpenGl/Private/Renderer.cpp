#include "Engine/Render/OpenGl/Renderer.hpp"

#include <SDL_video.h>

#include <entt/entity/registry.hpp>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/OpenGl/Model/RenderBatch.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"
#include "Engine/Render/OpenGl/Window.hpp"

#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Render/OpenGl/ECS/Components/RenderComponent.hpp"

SHV::OpenGl::Renderer::Renderer(SHV::OpenGl::Window& aWindow)
    : SHV::Renderer(), window(aWindow){};

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

void SHV::OpenGl::Renderer::Draw(const Scene& scene) {
    const auto renderView =
        scene.GetRegistry()
            .view<SHV::RenderComponent, SHV::TransformComponent>();

    for (const auto& [entity, renderComponent, transformComponent] :
         renderView.each()) {
        const auto& openGlRenderComponent =
            scene.GetRegistry().try_get<SHV::OpenGl::RenderComponent>(entity);
        AssertE(openGlRenderComponent != nullptr);

        const auto& renderBatch = openGlRenderComponent->renderBatch;

        program->Use();
        renderBatch.Bind();
        glDrawArrays(GL_TRIANGLES, 0, renderBatch.GetVertexCount());
    }
}

void SHV::OpenGl::Renderer::BeginFrame() {
    const auto viewportSize = window.GetViewportSize();
    glViewport(0, 0, viewportSize.x, viewportSize.y);

    glClearColor(window.GetWindowConfig().clearColor.r,
                 window.GetWindowConfig().clearColor.g,
                 window.GetWindowConfig().clearColor.b,
                 window.GetWindowConfig().clearColor.a);

    glClear(GL_COLOR_BUFFER_BIT);
};

void SHV::OpenGl::Renderer::EndFrame() {
    SDL_GL_SwapWindow(&window.GetWindow());
};

std::optional<std::string> SHV::OpenGl::Renderer::ValidateExtensions() {
    return std::nullopt;

    //    This is the place for validating extensions like this:
    //    if (!SDL_GL_ExtensionSupported("GL_ARB_enhanced_layouts")) {
    //        return "GL_ARB_enhanced_layouts";
    //    }
}