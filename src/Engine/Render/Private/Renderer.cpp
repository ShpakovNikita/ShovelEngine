#include "Engine/Render/Renderer.hpp"

SHV::Renderer::Renderer(const SHV::eRenderApi &aRenderApi)
    : renderApi(aRenderApi) {}

void SHV::Renderer::SetUp() {
    switch (renderApi) {
        case kMetal:
            break;
        case kOpenGLES:
            break;
    }
}

void SHV::Renderer::TearDown() {}