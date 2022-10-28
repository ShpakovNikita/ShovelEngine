#include "Engine/ECS/Systems/CameraSystem.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/Core/Window.hpp"

using namespace SHV;

CameraSystem::CameraSystem(entt::registry& registry, Window& aWindow)
    : System(registry), window(aWindow) {}

CameraSystem::~CameraSystem() = default;

void CameraSystem::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "CameraSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));

    auto renderView = registry.view<SHV::CameraComponent>();
    const glm::vec2 viewportSize = window.GetWindowSize();

    for (auto&& [entity, cameraComponent] : renderView.each()) {
        cameraComponent.projection =
            glm::perspective(cameraComponent.cameraFOV,
                             viewportSize.x / viewportSize.y, 0.1f, 100.0f);
    }
}