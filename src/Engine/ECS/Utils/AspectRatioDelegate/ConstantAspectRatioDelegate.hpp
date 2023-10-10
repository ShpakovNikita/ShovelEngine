#pragma once

#include "Engine/ECS/Utils/AspectRatioDelegate/AspectRatioDelegate.hpp"

namespace SHV {
class ConstantAspectRatioDelegate: public AspectRatioDelegate {
   public:
    ConstantAspectRatioDelegate(float aRatio) : ratio(aRatio) {}
    virtual ~ConstantAspectRatioDelegate() = default;

    virtual float GetAspectRatio() {
        return ratio;
    }
   private:
    float ratio;
};
}  // namespace SHV