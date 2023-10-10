#include "Engine/ECS/Systems/CameraSystem.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/ECS/Utils/AspectRatioDelegate/AspectRatioDelegate.hpp"

using namespace SHV;

CameraSystem::CameraSystem(entt::registry& registry, std::unique_ptr<AspectRatioDelegate> aDelegate)
    : System(registry), delegate(std::move(aDelegate)) {}

CameraSystem::~CameraSystem() = default;

void CameraSystem::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "CameraSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));

    auto renderView = registry.view<SHV::CameraComponent>();

    for (auto&& [entity, cameraComponent] : renderView.each()) {
        cameraComponent.projection = glm::perspective(cameraComponent.cameraFOV,
                             delegate->GetAspectRatio(), 0.1f, 100.0f);
    }
}