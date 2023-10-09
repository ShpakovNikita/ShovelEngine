#include "Engine/ECS/Utils/ObjectsCreationUtils.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"

#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"

#include "Engine/Render/ECS/Components/RenderComponent.hpp"

using namespace SHV;

entt::entity ObjectCreationUtils::CreateCube(entt::registry& registry, float edgeSize) {
    auto entity = registry.create();

    registry.emplace<TransformComponent>(entity);
    auto& renderComponent = registry.emplace<RenderComponent>(entity);
    renderComponent.primitive = ObjectCreationUtils::CreateCubePrimitive(edgeSize);

    return entity;
}

Primitive ObjectCreationUtils::CreateCubePrimitive(float edgeSize) {
    Primitive primitive = {};
    float halfSize = edgeSize / 2.0f;

    primitive.positions = {{-1.0, -1.0, 1.0, 1.0},  {1.0, -1.0, 1.0, 1.0},
                           {-1.0, 1.0, 1.0, 1.0},   {1.0, 1.0, 1.0, 1.0},
                           {1.0, 1.0, 1.0, 1.0},    {1.0, -1.0, 1.0, 1.0},
                           {1.0, 1.0, -1.0, 1.0},   {1.0, -1.0, -1.0, 1.0},
                           {-1.0, 1.0, 1.0, 1.0},   {1.0, 1.0, 1.0, 1.0},
                           {-1.0, 1.0, -1.0, 1.0},  {1.0, 1.0, -1.0, 1.0},
                           {1.0, -1.0, 1.0, 1.0},   {-1.0, -1.0, 1.0, 1.0},
                           {1.0, -1.0, -1.0, 1.0},  {-1.0, -1.0, -1.0, 1.0},
                           {-1.0, -1.0, 1.0, 1.0},  {-1.0, 1.0, 1.0, 1.0},
                           {-1.0, -1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0, 1.0},
                           {-1.0, -1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0, 1.0},
                           {1.0, -1.0, -1.0, 1.0},  {1.0, 1.0, -1.0, 1.0}};
    for (auto& position : primitive.positions) {
        position.x *= halfSize;
        position.y *= halfSize;
        position.z *= halfSize;
    }

    primitive.normals = {{0.0, 0.0},  {0.0, 0.0},  {0.0, 0.0},  {0.0, 0.0},
                         {1.0, 0.0},  {1.0, 0.0},  {1.0, 0.0},  {1.0, 0.0},
                         {0.0, 1.0},  {0.0, 1.0},  {0.0, 1.0},  {0.0, 1.0},
                         {0.0, -1.0}, {0.0, -1.0}, {0.0, -1.0}, {0.0, -1.0},
                         {-1.0, 0.0}, {-1.0, 0.0}, {-1.0, 0.0}, {-1.0, 0.0},
                         {0.0, 0.0},  {0.0, 0.0},  {0.0, 0.0},  {0.0, 0.0}};

    primitive.uvs = {{1.0, 0.0}, {0.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0},
                     {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0},
                     {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0},
                     {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}, {1.0, 1.0}, {0.0, 1.0},
                     {0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};

    primitive.indices = {0,  1,  2,  3,  2,  1,  4,  5,  6,  7,  6,  5,
                         8,  9,  10, 11, 10, 9,  12, 13, 14, 15, 14, 13,
                         16, 17, 18, 19, 18, 17, 20, 21, 22, 23, 22, 21};
    
    return primitive;
}
