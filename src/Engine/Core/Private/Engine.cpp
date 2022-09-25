#include "Engine/Core/Engine.hpp"

#include <SDL.h>

#include "Engine/Common/Exception.hpp"
#include "Engine/Core/ImmutableConfig.hpp"
#include "Engine/Utils/Logger.hpp"

SHV::Engine::Engine(const ImmutableConfig& aConfig) : config(aConfig) {}

int SHV::Engine::Run() noexcept {
    try {
        Init();
    } catch (SHV::Exception& e) {
        SHV::LogE(SHV::eTag::kBase)
            << "Critical error occurred during engine run! " << e.what();
        return EXIT_FAILURE;
    }

    // Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }

    return EXIT_SUCCESS;
}

void SHV::Engine::Init() {
    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SHV::Exception("SDL could not initialize! SDL_Error: %s\n",
                             SDL_GetError());
    } else {
        // Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, config.width,
                                  config.height, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
        } else {
            // Get window surface
            screenSurface = SDL_GetWindowSurface(window);

            // Fill the surface white
            SDL_FillRect(screenSurface, NULL,
                         SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

            // Update the surface
            SDL_UpdateWindowSurface(window);
        }
    }
}

SHV::Engine::~Engine() = default;
