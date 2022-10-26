#include "Engine/ECS/Utils/ObjectsCreationUtils.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"

#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"

#include "Engine/Render/ECS/Components/RenderComponent.hpp"

using namespace SHV;

entt::entity ObjectCreationUtils::CreateCube(entt::registry& registry) {
    auto entity = registry.create();

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->materialShader = SHV::eShader::kBasicShader;
    Primitive primitive = {material};

    primitive.positions = {{0.5f, 0.5f, 0.0f, 1.0}, {0.5f, -0.5f, 0.0f, 1},
                           {-0.5f, -0.5f, 0.0f, 1}, {-0.5f, 0.5f, 0.0f, 1},
                           {0.5f, 0.5f, 1.0f, 1.0}, {0.5f, -0.5f, 1.0f, 1},
                           {-0.5f, -0.5f, 1.0f, 1}, {-0.5f, 0.5f, 1.0f, 1}};
    primitive.normals = {{0, 1.0}, {0, 1.0}, {0, 1.0}, {0, 1.0},
                         {0, 1.0}, {0, 1.0}, {0, 1.0}, {0, 1.0}};
    primitive.uvs = {{1.0, 1.0}, {1.0, 0}, {0, 0}, {0, 1.0},
                     {1.0, 1.0}, {1.0, 0}, {0, 0}, {0, 1.0}};

    primitive.indices = {0, 1, 3, 1, 2, 3, 0, 1, 4, 1, 4, 5, 1, 5, 2, 2, 6, 5,
                         2, 6, 7, 7, 3, 2, 3, 7, 0, 0, 7, 3, 4, 5, 7, 5, 6, 7};

    auto& renderComponent = registry.emplace<RenderComponent>(entity);
    registry.emplace<TransformComponent>(entity);

    renderComponent.primitive = primitive;

    return entity;
}