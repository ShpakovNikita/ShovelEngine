#include "Engine/ECS/Systems/InputSystem.hpp"

#include "Engine/ECS/Components/InputComponent.hpp"

#include "Engine/Core/Input/InputManager.hpp"

using namespace SHV;

InputSystem::InputSystem(entt::registry& registry, InputManager& aInputManager)
    : System(registry), inputManager(aInputManager) {}

InputSystem::~InputSystem() = default;

void InputSystem::Process(float /*dt*/) {
    auto renderView = registry.view<SHV::InputComponent>();

    for (auto&& [entity, inputComponent] : renderView.each()) {
        inputComponent.input = inputManager.GetInput();
    }
}