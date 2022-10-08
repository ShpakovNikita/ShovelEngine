#pragma once

#include "Engine/ECS/System.hpp"

namespace SHV {
struct RenderComponent;

namespace OpenGl {
class RenderSystem : public ::SHV::System {
   public:
    RenderSystem(entt::registry& registry);
    virtual ~RenderSystem();

    virtual void Process(float dt);

    virtual void SetUp();
    virtual void TearDown();

   private:
    void AddRenderBatch(entt::entity& entity,
                        const SHV::RenderComponent& renderComponent);
};
}  // namespace OpenGl
}  // namespace SHV