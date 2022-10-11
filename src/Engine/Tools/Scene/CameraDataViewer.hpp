#pragma once

#include <entt/entt.hpp>

namespace SHV {
class CameraDataViewer {
   public:
    CameraDataViewer(entt::registry& registry);

    void Draw(bool* show);

   private:
    entt::registry& registry;

};
}