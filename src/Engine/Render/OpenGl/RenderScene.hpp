#pragma once

#include "Engine/Render/RenderBatchManager.hpp"

namespace SHV::OpenGl {
class RenderBatch;

class RenderScene {
   public:
    const RenderBatchManager<RenderBatch>& GetRenderBatchManager() const;

   private:
    std::unique_ptr<RenderBatchManager<RenderBatch>> renderBatchManager;
};
}  // namespace SHV::OpenGl