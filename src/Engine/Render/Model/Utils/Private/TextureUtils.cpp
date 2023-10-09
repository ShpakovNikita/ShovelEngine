#include "Engine/Render/Model/Utils/TextureUtils.hpp"

#include <cstdlib>
#include <cstring>

using namespace SHV;

uint8_t* TextureUtils::RemapTextureData(
    const uint8_t* srcImageData, uint32_t width, uint32_t height,
    uint32_t srcChannelsCount, uint32_t dstChannelsCount,
    uint32_t srcBytesPerPixel, uint32_t dstBytesPerPixel, void* fillValue,
    bool flipX, bool flipY) {
    unsigned char* dstImageData = static_cast<unsigned char*>(
        std::malloc(width * height * dstChannelsCount * dstBytesPerPixel));

    // For every row of the image
    for (uint32_t y = 0; y < height; ++y) {
        uint32_t srcRow = flipY ? height - y - 1 : y;

        // For every column of the current row
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t srcColumn = flipX ? width - x - 1 : x;

            // Calculate the index for the first byte of the pixel you're
            // converting in both the source and destination images
            uint32_t srcPixelIndex = srcChannelsCount * srcBytesPerPixel *
                                     (srcRow * width + srcColumn);
            uint32_t dstPixelIndex =
                dstChannelsCount * dstBytesPerPixel * (y * width + x);

            if (dstChannelsCount > srcChannelsCount) {
                for (uint32_t j = 0; j < srcChannelsCount; ++j) {
                    std::memcpy(&dstImageData[dstPixelIndex + j * dstBytesPerPixel],
                                &srcImageData[srcPixelIndex + j * srcBytesPerPixel],
                                dstBytesPerPixel);
                }

                for (uint32_t j = 0; j < dstChannelsCount - srcChannelsCount;
                     ++j) {
                    std::memcpy(
                        &dstImageData[dstPixelIndex + (srcChannelsCount + j) * dstBytesPerPixel],
                        fillValue, dstBytesPerPixel);
                }
            } else {
                for (uint32_t j = 0; j < dstChannelsCount; ++j) {
                    std::memcpy(&dstImageData[dstPixelIndex + j * dstBytesPerPixel],
                                &srcImageData[srcPixelIndex + j * srcBytesPerPixel],
                                dstBytesPerPixel);
                }
            }
        }
    }

    return dstImageData;
}

uint8_t* TextureUtils::RemapTextureDataOneByte(
    const uint8_t* srcImageData, uint32_t width, uint32_t height,
    uint32_t srcChannelsCount, uint32_t dstChannelsCount,
    unsigned char fillValue, bool flipX, bool flipY) {
    return RemapTextureData(srcImageData, width, height, srcChannelsCount,
                            dstChannelsCount, 1, 1, &fillValue, flipX, flipY);
}

uint8_t* TextureUtils::RemapTextureDataFloat(
    const uint8_t* srcImageData, uint32_t width, uint32_t height,
    uint32_t srcChannelsCount, uint32_t dstChannelsCount, float fillValue,
    bool flipX, bool flipY) {
    return RemapTextureData(srcImageData, width, height, srcChannelsCount,
                            dstChannelsCount, sizeof(float), sizeof(float),
                            &fillValue, flipX, flipY);
}
