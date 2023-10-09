#pragma once

#include "Engine/Core/Input/Input.hpp"

#include "SDL_events.h"

namespace SHV {
class InputManager {
   public:
    void PollEvents(SDL_Event* event);
    void ClearInput();
    
    void TearDown();
    void SetUp();

    inline const Input& GetInput() const { return input; };

   private:
    void Reset();
    
    Input input;
};
}  // namespace SHV
