#pragma once

#include <entt/entity/registry.hpp>

namespace SHV {
struct RelationshipComponent {
    entt::entity first{entt::null};
    entt::entity prev{entt::null};
    entt::entity next{entt::null};
    entt::entity parent{entt::null};
};
}  // namespace SHV