#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/ECS/System.hpp"

#include <memory>
#include <functional>
#include <vector>

namespace SHV {

class Scene {
   public:
    Scene();
    ~Scene();

    void SceneGraphTraversal(
        const std::function<void(entt::entity&)>& entityProcessor);

    void Process(float dt);

    void SetUp();
    void TearDown();

    entt::registry& GetRegistry();
    const entt::registry& GetRegistry() const;

    const entt::entity& GetRootEntity() const;

    template <class SystemType, typename... Args>
    SystemType& AddSystem(Args&&... args);

   private:
    void SceneGraphTraversal(
        entt::entity& entity,
        const std::function<void(entt::entity&)>& entityProcessor);

    entt::registry registry;
    entt::entity rootEntity;

    std::vector<std::unique_ptr<System>> systems;
};
}  // namespace SHV

template <class SystemType, typename... Args>
SystemType& SHV::Scene::AddSystem(Args&&... args) {
    std::unique_ptr<SystemType> system =
        std::make_unique<SystemType>(registry, std::forward<Args>(args)...);
    systems.push_back(std::move(system));
    return static_cast<SystemType&>(*systems.back());
}
