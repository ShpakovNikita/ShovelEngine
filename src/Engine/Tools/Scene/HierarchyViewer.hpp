#pragma once

#include <entt/entt.hpp>

namespace SHV {
class HierarchyViewer {
   public:
    HierarchyViewer(entt::registry& registry);

    void Draw(bool* show);

   private:
    void DrawNode(entt::entity& entity);

    entt::registry& registry;
};
}  // namespace SHV