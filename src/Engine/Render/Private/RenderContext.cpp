#include "Engine/Render/RenderContext.hpp"

#include "Engine/Core/Window.hpp"
#include "Engine/Common/Assert.hpp"
#include "Engine/Render/ImGuiImpl.hpp"
#include "Engine/Render/Metal/Renderer.hpp"
#include "Engine/Render/Metal/WindowContext.hpp"
#include "Engine/Render/OpenGl/Renderer.hpp"
#include "Engine/Render/OpenGl/WindowContext.hpp"

using namespace SHV;

SHV::RenderContext::RenderContext(const WindowConfig& aConfig,
                                  const eRenderApi& aRenderApi)
    : renderer(nullptr),
      window(nullptr),
      imGui(nullptr),
      windowConfig(aConfig),
      renderApi(aRenderApi){};

SHV::RenderContext::~RenderContext() = default;

SHV::Renderer& SHV::RenderContext::GetRenderer() const {
    AssertD(renderer != nullptr);
    return *renderer;
}

SHV::ImGui& SHV::RenderContext::GetImGui() const {
    AssertD(imGui != nullptr);
    return *imGui;
}

SHV::WindowContext& SHV::RenderContext::GetWindowContext() const {
    AssertD(windowContext != nullptr);
    return *windowContext;
}

SHV::Window& SHV::RenderContext::GetWindow() const {
    AssertD(window != nullptr);
    return *window;
}

void SHV::RenderContext::SwitchRenderApi(const eRenderApi& aRenderApi) {
    if (aRenderApi != renderApi) {
        AssertD(window != nullptr);
        AssertD(renderer != nullptr);
        AssertD(windowContext != nullptr);

        renderApi = aRenderApi;

        imGui->TearDown();
        renderer->TearDown();
        windowContext->TearDown();

        switch (renderApi) {
            case eRenderApi::kMetal:
                CreateMetalApiContext();
                break;
            case eRenderApi::kOpenGLES:
                CreateOpenGlContext();
                break;
        }

        windowContext->SetUp();
        renderer->SetUp();
        imGui->SetUp();
    }
}

const SHV::eRenderApi& SHV::RenderContext::GetRenderApi() const {
    return renderApi;
}

void SHV::RenderContext::SetUp() {
    // Create window
    switch (renderApi) {
        case eRenderApi::kMetal:
            CreateMetalApiContext();
            break;
        case eRenderApi::kOpenGLES:
            CreateOpenGlContext();
            break;
    }

    AssertD(windowContext != nullptr);
    AssertD(window != nullptr);
    AssertD(renderer != nullptr);

    window->SetUp();
    windowContext->SetUp();
    renderer->SetUp();

    imGui = std::make_unique<ImGui>(*this);
    imGui->SetUp();
}

void SHV::RenderContext::TearDown() {
    AssertD(windowContext != nullptr);
    AssertD(window != nullptr);
    AssertD(renderer != nullptr);

    imGui->TearDown();
    imGui = nullptr;

    renderer->TearDown();
    windowContext->TearDown();
    window->TearDown();
}

void SHV::RenderContext::CreateMetalApiContext() {
    window = std::make_unique<Window>(windowConfig);
    windowContext = std::make_unique<Metal::WindowContext>(*window);
    renderer = std::make_unique<Metal::Renderer>(
        *static_cast<Metal::WindowContext*>(windowContext.get()));
}

void SHV::RenderContext::CreateOpenGlContext() {
    window = std::make_unique<Window>(windowConfig);
    windowContext = std::make_unique<OpenGl::WindowContext>(*window);
    renderer = std::make_unique<OpenGl::Renderer>(
        *static_cast<OpenGl::WindowContext*>(windowContext.get()));
}