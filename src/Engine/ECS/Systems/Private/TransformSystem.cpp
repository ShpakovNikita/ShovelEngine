#include "Engine/ECS/Systems/TransformSystem.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

using namespace SHV;

void TransformSystem::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "TransformSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));
}