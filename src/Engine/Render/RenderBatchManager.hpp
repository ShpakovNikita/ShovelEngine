#pragma once

#include <map>
#include <memory>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Render/Model/RenderBatchHandle.h"

namespace SHV {

template <typename RenderBatchType>
class RenderBatchManager {
   public:
    RenderBatchManager() = default;
    ~RenderBatchManager() = default;

    RenderBatchType* GetRenderBatch(
        const SHV::RenderBatchHandle& batchHandle) const;

    SHV::RenderBatchHandle AddRenderBatch(
        std::unique_ptr<RenderBatchType> renderBatch);
    void RemoveRenderBatch(const SHV::RenderBatchHandle& batchHandle);

   private:
    // TODO: implement
    void InvalidateRenderBatches();

    std::map<SHV::RenderBatchHandle, std::unique_ptr<RenderBatchType>>
        renderBatches;
    SHV::RenderBatchHandle currentHandle =
        std::numeric_limits<RenderBatchHandle>::min();
};

template <typename RenderBatchType>
RenderBatchType* SHV::RenderBatchManager<RenderBatchType>::GetRenderBatch(
    const SHV::RenderBatchHandle& batchHandle) const {
    const auto renderBatch = renderBatches.find(batchHandle);

    AssertD(renderBatch != renderBatches.end());

    return renderBatch->second.get();
}

template <typename RenderBatchType>
void SHV::RenderBatchManager<RenderBatchType>::RemoveRenderBatch(
    const SHV::RenderBatchHandle& batchHandle) {
    renderBatches.erase(batchHandle);
}

template <typename RenderBatchType>
SHV::RenderBatchHandle SHV::RenderBatchManager<RenderBatchType>::AddRenderBatch(
    std::unique_ptr<RenderBatchType> renderBatch) {
    AssertD(currentHandle != SHV::kInvalidBatchHandle);

    renderBatches.insert({currentHandle, std::move(renderBatch)});
    SHV::RenderBatchHandle acquiredHandle = currentHandle++;

    if (currentHandle != SHV::kInvalidBatchHandle) {
        InvalidateRenderBatches();
    }

    return acquiredHandle;
}

template <typename RenderBatchType>
void SHV::RenderBatchManager<RenderBatchType>::InvalidateRenderBatches() {
    throw SHV::Exception("Render Batches Invalidation not implemented!");
}
}  // namespace SHV
