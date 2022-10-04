#pragma once

#include <entt/entity/registry.hpp>

#include <memory>
#include <functional>

namespace SHV {
class Scene {
   public:
    Scene();
    ~Scene();

    void SceneGraphTraversal(
        const std::function<void(entt::entity&)>& entityProcessor);

    const entt::registry& GetRegistry() const;
    const entt::entity& GetRootEntity() const;

   private:
    void SceneGraphTraversal(
        entt::entity& entity,
        const std::function<void(entt::entity&)>& entityProcessor);

    entt::registry registry;
    entt::entity rootEntity;
};
}  // namespace SHV
