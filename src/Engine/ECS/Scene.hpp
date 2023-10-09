#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/ECS/System.hpp"
#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Memory.hpp"

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
    void SceneGraphTraversal(
        const std::function<void(const entt::entity&)>& entityProcessor) const;

    void Process(float dt);

    entt::registry& GetRegistry();
    const entt::registry& GetRegistry() const;

    entt::entity& GetRootEntity();
    const entt::entity& GetRootEntity() const;

    entt::entity GetEntityWithActiveCamera() const;

    template <class SystemType, typename... Args>
    SystemType& AddSystem(Args&&... args);

    template <class SystemType>
    void RemoveSystem();

   private:
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
    SystemType& systemRef = static_cast<SystemType&>(*systems.back());
    systemRef.SetUp();
    return systemRef;
}

template <class SystemType>
void SHV::Scene::RemoveSystem() {
    auto it = find_if(systems.begin(), systems.end(), [](const auto& system) {
        return dynamic_cast<SystemType*>(system.get()) != nullptr;
    });

    AssertD(it != systems.end());
    (*it)->TearDown();
    systems.erase(it);
}
