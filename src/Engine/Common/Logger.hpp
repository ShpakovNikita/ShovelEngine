#pragma once

#include <iostream>
#include <string>

namespace SHV {
enum class eTag {
    kBase = 0,
    kRender,
    kMetalAPI,
    kOpenGl,
    kAssetLoading,
    kEngine,
};

// TODO: return logger instance
std::ostream& LogD(eTag tag);
std::ostream& LogI(eTag tag);
std::ostream& LogW(eTag tag);
std::ostream& LogE(eTag tag);
}  // namespace SHV
