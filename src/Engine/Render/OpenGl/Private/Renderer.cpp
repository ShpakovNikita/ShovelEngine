#include "Engine/Render/OpenGl/Renderer.hpp"

#include <vector>

#include "Engine/Common/Exception.hpp"

SHV::OpenGl::Renderer::Renderer(Window& /*window*/) : SHV::Renderer(){};

void SHV::OpenGl::Renderer::SetUp() {
    throw SHV::Exception("Open GL Renderer not implemented!");
}

void SHV::OpenGl::Renderer::TearDown() {
    throw SHV::Exception("Open GL Renderer not implemented!");
}

void SHV::OpenGl::Renderer::Draw() {
    throw SHV::Exception("Open GL Renderer not implemented!");
}
