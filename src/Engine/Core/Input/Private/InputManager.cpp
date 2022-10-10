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

void InputManager::PollEvents(SDL_Event* event) {
    Input currentInput;

    switch (event->type) {
        /* Keyboard event */
        case SDL_KEYDOWN: {
            auto keyIt =
                SInputManager::kKeySDLToSHV.find(event->key.keysym.sym);
            if (keyIt != SInputManager::kKeySDLToSHV.end()) {
                currentInput.pressedKeys[static_cast<size_t>(keyIt->second)] =
                    true;
            }
            break;
        }
        case SDL_KEYUP: {
            auto keyIt =
                SInputManager::kKeySDLToSHV.find(event->key.keysym.sym);
            if (keyIt != SInputManager::kKeySDLToSHV.end()) {
                currentInput.pressedKeys[static_cast<size_t>(keyIt->second)] =
                    false;
            }
            break;
        }
        default:
            break;
    }

    input = currentInput;
}