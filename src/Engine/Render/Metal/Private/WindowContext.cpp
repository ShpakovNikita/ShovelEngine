#include "Engine/Render/Metal/WindowContext.hpp"

#include "Engine/Core/Window.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"
#include "Engine/Render/Metal/MetalBridge.hpp"

SHV::Metal::WindowContext::WindowContext(const SHV::Window& aWindow)
    : ::SHV::WindowContext(), window(aWindow){};

SHV::Metal::WindowContext::~WindowContext() = default;

const SHV::Window& SHV::Metal::WindowContext::GetWindow() { return window; }

const SDL_MetalView& SHV::Metal::WindowContext::GetMetalView() const {
    AssertD(metalView != nullptr);
    return metalView;
}

void* SHV::Metal::WindowContext::GetCAMetalLayer() const {
    void* layer = SDL_Metal_GetLayer(metalView);
    AssertD(layer != nullptr);
    return layer;
}

CA::MetalDrawable* SHV::Metal::WindowContext::NextDrawable() const {
    return SHV::Metal::NextDrawable(GetCAMetalLayer());
}

void SHV::Metal::WindowContext::SetUp() {
    LogD(eTag::kMetalAPI) << "Setting up WindowContext" << std::endl;

    metalView = SDL_Metal_CreateView(&window.GetWindow());
}

void SHV::Metal::WindowContext::TearDown() {
    SDL_Metal_DestroyView(metalView);

    LogD(eTag::kMetalAPI) << "WindowContext teared down" << std::endl;
}