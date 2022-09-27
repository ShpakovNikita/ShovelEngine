#include "Engine/Render/OpenGl/Window.hpp"

#include "Engine/Common/Exception.hpp"

SHV::OpenGl::Window::Window(const WindowConfig& config) : SHV::Window(config){};

void SHV::OpenGl::Window::OnSetUpComplete() {
    throw SHV::Exception("Open GL Window not implemented!");
}

void SHV::OpenGl::Window::OnTearDownBegin() {
    throw SHV::Exception("Open GL Window not implemented!");
}