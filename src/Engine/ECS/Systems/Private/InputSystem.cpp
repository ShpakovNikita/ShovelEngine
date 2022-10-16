#include "Engine/ECS/Systems/InputSystem.hpp"

#include "Engine/ECS/Components/InputComponent.hpp"

#include "Engine/Core/Input/InputManager.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

using namespace SHV;

InputSystem::InputSystem(entt::registry& registry, InputManager& aInputManager)
    : System(registry), inputManager(aInputManager) {}

InputSystem::~InputSystem() = default;

void InputSystem::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "InputSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));

    auto renderView = registry.view<SHV::InputComponent>();

    for (auto&& [entity, inputComponent] : renderView.each()) {
        inputComponent.input = inputManager.GetInput();
    }
}