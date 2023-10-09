#include "Engine/Core/Input/InputManager.hpp"

#include <map>

using namespace SHV;

namespace SHV::SInputManager {
static std::map<SDL_Keycode, SHV::eKey> kKeySDLToSHV = {
    {SDLK_w, eKey::kW},
    {SDLK_a, eKey::kA},
    {SDLK_s, eKey::kS},
    {SDLK_d, eKey::kD},
};
}

void InputManager::ClearInput() {
    input.mouseMotion.dx = 0;
    input.mouseMotion.dy = 0;
}

void InputManager::Reset() {
    ClearInput();
    
    input.pressedKeys = {};
}

void InputManager::TearDown() {
    Reset();
}

void InputManager::SetUp() {
    Reset();
}

void InputManager::PollEvents(SDL_Event* event) {
    switch (event->type) {
        /* Keyboard event */
        case SDL_KEYDOWN: {
            auto keyIt =
                SInputManager::kKeySDLToSHV.find(event->key.keysym.sym);
            if (keyIt != SInputManager::kKeySDLToSHV.end()) {
                input.pressedKeys[static_cast<size_t>(keyIt->second)] = true;
            }
            break;
        }
        case SDL_KEYUP: {
            auto keyIt =
                SInputManager::kKeySDLToSHV.find(event->key.keysym.sym);
            if (keyIt != SInputManager::kKeySDLToSHV.end()) {
                input.pressedKeys[static_cast<size_t>(keyIt->second)] = false;
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            MouseMotion mouseMotion{event->motion.x, event->motion.y,
                                    event->motion.xrel, event->motion.yrel};
            input.mouseMotion = mouseMotion;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if (event->button.button == SDL_BUTTON_RIGHT) {
                input.pressedKeys[static_cast<size_t>(eKey::kRMB)] = true;
            } else if (event->button.button == SDL_BUTTON_LEFT) {
                input.pressedKeys[static_cast<size_t>(eKey::kLMB)] = true;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            if (event->button.button == SDL_BUTTON_RIGHT) {
                input.pressedKeys[static_cast<size_t>(eKey::kRMB)] = false;
            } else if (event->button.button == SDL_BUTTON_LEFT) {
                input.pressedKeys[static_cast<size_t>(eKey::kLMB)] = false;
            }
            break;
        }
        default:
            break;
    }
}
