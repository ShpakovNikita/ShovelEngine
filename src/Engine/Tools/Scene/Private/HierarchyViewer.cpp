#include "Engine/Tools/Scene/HierarchyViewer.hpp"

#include "Engine/Common/Assert.hpp"

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/RootComponent.hpp"
#include "Engine/ECS/Components/NameComponent.hpp"

#include "imgui.h"

#include <sstream>

using namespace SHV;

namespace SHierarchyViewer {
    std::string GetNodeName(entt::entity& entity, entt::registry& registry) {
        auto nameComponent = registry.try_get<NameComponent>(entity);
        if (nameComponent != nullptr) {
            return nameComponent->name;
        }
        std::ostringstream oss;
        oss << "<entity_node_" << static_cast<uint64_t>(entity) << ">";
        return oss.str();
    }
}

HierarchyViewer::HierarchyViewer(entt::registry& aRegistry)
    : registry(aRegistry) {}

void HierarchyViewer::Draw(bool* show) {
    if (!ImGui::Begin("Hierarchy viewer", show))
    {
        ImGui::End();
        return;
    }

    auto rootComponentView = registry.view<RootComponent>();
    AssertD(rootComponentView.size() == 1);

    for (auto&& [sceneRoot] : rootComponentView.each()) {
        DrawNode(sceneRoot);
    }

    ImGui::End();
}

void HierarchyViewer::DrawNode(entt::entity& entity) {
    const auto child = registry.get<RelationshipComponent>(entity).first;

    // We are in leaf
    if (child == entt::null) {
        std::string name = SHierarchyViewer::GetNodeName(entity, registry);
        ImGui::BulletText(name.c_str(), "");
        return;
    }

    std::string name = SHierarchyViewer::GetNodeName(entity, registry);
    bool nodeOpen =
        ImGui::TreeNodeEx((void*)(intptr_t)entity, 0, name.c_str(), "");

    if (nodeOpen) {
        auto curr = child;

        while (curr != entt::null) {
            DrawNode(curr);
            curr = registry.get<RelationshipComponent>(curr).next;
        }

        ImGui::TreePop();
    }
}