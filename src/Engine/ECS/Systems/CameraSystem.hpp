#pragma once

#include "Engine/ECS/System.hpp"

#include <memory>

namespace SHV {

class AspectRatioDelegate;

class CameraSystem : public System {
   public:
    CameraSystem(entt::registry& registry, std::unique_ptr<AspectRatioDelegate> delegate);
    virtual ~CameraSystem();

    virtual void Process(float dt);

   private:
    std::unique_ptr<AspectRatioDelegate> delegate;
};
}  // namespace SHV