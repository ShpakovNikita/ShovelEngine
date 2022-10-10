#include "Engine/ECS/Systems/CameraSystem.hpp"

#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/Core/Window.hpp"

using namespace SHV;

CameraSystem::CameraSystem(entt::registry& registry, Window& aWindow)
    : System(registry), window(aWindow) {}

CameraSystem::~CameraSystem() = default;

void CameraSystem::Process(float /*dt*/) {
    auto renderView = registry.view<SHV::CameraComponent>();
    const glm::vec2 viewportSize = window.GetViewportSize();

    for (auto&& [entity, cameraComponent] : renderView.each()) {
        cameraComponent.projection = glm::perspective(
            glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.1f, 100.0f);
    }
}