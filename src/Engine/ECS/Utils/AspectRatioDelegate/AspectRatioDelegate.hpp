#pragma once

#include "Engine/ECS/System.hpp"

#include <memory>

namespace SHV {
class AspectRatioDelegate {
   public:
    virtual float GetAspectRatio() = 0;
    virtual ~AspectRatioDelegate() = default;
};
}  // namespace SHV