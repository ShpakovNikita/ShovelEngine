#pragma once

#include "Engine/ECS/System.hpp"
#include <memory>

namespace SHV {
class RenderContext;
class Texture;
struct RenderComponent;
struct SkyboxComponent;

class SkyboxSystem : public System {
   public:
    SkyboxSystem(entt::registry& registry, RenderContext& renderContext);
    virtual ~SkyboxSystem();

    virtual void Process(float dt);

   private:
    void CreateSkyboxCubeMap(entt::entity entity);
    std::shared_ptr<Texture> CreateCubeMapTextureFromEquirectangularProjection(std::shared_ptr<Texture> texture);
    
    RenderContext& renderContext;
};
}  // namespace SHV
