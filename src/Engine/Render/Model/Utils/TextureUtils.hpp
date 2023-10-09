#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdint>

namespace SHV {
namespace TextureUtils {
/*
 * This function remaps source image data with according parameters
 *
 * srcBytesPerPixel and dstBytesPerPixel should be equals for predictable
 * result, unless you know what you are doing
 */
uint8_t* RemapTextureData(const uint8_t* srcImageData,
                                uint32_t width, uint32_t height,
                                uint32_t srcChannelsCount,
                                uint32_t dstChannelsCount,
                                uint32_t srcBytesPerPixel,
                                uint32_t dstBytesPerPixel, void* fillValue,
                                bool flipX, bool flipY);

uint8_t* RemapTextureDataOneByte(const uint8_t* srcImageData,
                                       uint32_t width, uint32_t height,
                                       uint32_t srcChannelsCount,
                                       uint32_t dstChannelsCount,
                                       unsigned char fillValue = 255,
                                       bool flipX = false, bool flipY = false);

uint8_t* RemapTextureDataFloat(const uint8_t* srcImageData,
                                     uint32_t width, uint32_t height,
                                     uint32_t srcChannelsCount,
                                     uint32_t dstChannelsCount,
                                     float fillValue = 1.0f, bool flipX = false,
                                     bool flipY = false);
}  // namespace TextureUtils
}  // namespace SHV
