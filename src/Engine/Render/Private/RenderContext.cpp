#include "Engine/Render/RenderContext.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Metal/Renderer.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Engine/Render/OpenGl/Renderer.hpp"
#include "Engine/Render/OpenGl/Window.hpp"

SHV::RenderContext::RenderContext(const WindowConfig& aConfig,
                                  const eRenderApi& aRenderApi)
    : renderer(nullptr),
      window(nullptr),
      windowConfig(aConfig),
      renderApi(aRenderApi){};

SHV::RenderContext::~RenderContext() = default;

SHV::Renderer& SHV::RenderContext::GetRenderer() const {
    AssertD(renderer != nullptr);
    return *renderer;
}

SHV::Window& SHV::RenderContext::GetWindow() const {
    AssertD(window != nullptr);
    return *window;
}

void SHV::RenderContext::SetUp() {
    // Create window
    switch (renderApi) {
        case kMetal:
            CreateMetalApiContext();
            break;
        case kOpenGLES:
            CreateOpenGlContext();
            break;
    }

    AssertD(window != nullptr);
    AssertD(renderer != nullptr);

    window->SetUp();
    renderer->SetUp();
}

void SHV::RenderContext::TearDown() {
    AssertD(window != nullptr);
    AssertD(renderer != nullptr);

    renderer->TearDown();
    window->TearDown();
}

void SHV::RenderContext::CreateMetalApiContext() {
    window = std::make_unique<Metal::Window>(windowConfig);
    renderer = std::make_unique<Metal::Renderer>(
        *static_cast<Metal::Window*>(window.get()));
}

void SHV::RenderContext::CreateOpenGlContext() {
    window = std::make_unique<OpenGl::Window>(windowConfig);
    renderer = std::make_unique<OpenGl::Renderer>(
        *static_cast<OpenGl::Window*>(window.get()));
}