#include "Engine/Tools/Scene/HierarchyViewer.hpp"

#include "Engine/Common/Assert.hpp"

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/RootComponent.hpp"

#include "imgui.h"

using namespace SHV;

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
        ImGui::BulletText("Entity Node %i", entity);
        return;
    }

    bool nodeOpen =
        ImGui::TreeNodeEx((void*)(intptr_t)entity, 0, "Entity Node %i", entity);

    if (nodeOpen) {
        auto curr = child;

        while (curr != entt::null) {
            DrawNode(curr);
            curr = registry.get<RelationshipComponent>(curr).next;
        }

        ImGui::TreePop();
    }
}