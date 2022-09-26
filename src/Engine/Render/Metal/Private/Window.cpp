#include "Engine/Render/Metal/Window.hpp"

#include "Engine/Render/Metal/MetalBridge.hpp"
#include "Engine/Utils/Assert.hpp"

SHV::Metal::Window::Window(const WindowConfig& config) : SHV::Window(config){};

const SDL_MetalView& SHV::Metal::Window::GetMetalView() const {
    AssertD(metalView != nullptr);
    return metalView;
}

void* SHV::Metal::Window::GetCAMetalLayer() const {
    void* layer = SDL_Metal_GetLayer(metalView);
    AssertD(layer != nullptr);
    return layer;
}

CA::MetalDrawable* SHV::Metal::Window::NextDrawable() const {
    return SHV::Metal::NextDrawable(GetCAMetalLayer());
}

void SHV::Metal::Window::OnSetUpComplete() {
    metalView = SDL_Metal_CreateView(&GetWindow());
}

void SHV::Metal::Window::OnTearDownBegin() { SDL_Metal_DestroyView(metalView); }