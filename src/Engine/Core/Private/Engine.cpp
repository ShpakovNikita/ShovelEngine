#include "Engine/Core/Engine.hpp"

#include <SDL.h>

#include "Engine/Common/Exception.hpp"
#include "Engine/Core/ImmutableConfig.hpp"
#include "Engine/Render/Metal/Renderer.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Engine/Utils/Logger.hpp"

using namespace SHV;

Engine::Engine(const ImmutableConfig& aConfig)
    : config(aConfig), renderer(nullptr), window(nullptr) {}

int Engine::Run() noexcept {
    try {
        SetUp();
    } catch (SHV::Exception& e) {
        SHV::LogE(SHV::eTag::kBase)
            << "Critical error occurred during engine SetUp! " << e.what();
        return EXIT_FAILURE;
    }

    // Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            renderer->Draw();
            if (e.type == SDL_QUIT) quit = true;
        }
    }

    try {
        TearDown();
    } catch (SHV::Exception& e) {
        SHV::LogE(SHV::eTag::kBase)
            << "Critical error occurred during engine TearDown! " << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Engine::SetUp() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Exception("SDL could not initialize! SDL_Error: %s\n",
                        SDL_GetError());
    }

    // Create window

    const WindowConfig windowConfig = {config.width, config.height};
    window = std::make_unique<Metal::Window>(windowConfig);
    window->SetUp();

    renderer = std::make_unique<Metal::Renderer>(*window.get());
    renderer->SetUp();

    // Get window surface
    screenSurface = SDL_GetWindowSurface(&window->GetWindow());

    // Fill the surface white
    SDL_FillRect(screenSurface, nullptr,
                 SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(&window->GetWindow());
}

void Engine::TearDown() {
    renderer->TearDown();
    window->TearDown();

    SDL_Quit();
}

Engine::~Engine() = default;
