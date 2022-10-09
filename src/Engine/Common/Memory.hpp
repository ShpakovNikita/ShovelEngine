#pragma once

#include <memory>

namespace SHV {
template <typename To, typename From, typename Deleter>
std::unique_ptr<To, Deleter> DynamicUniqueCast(
    std::unique_ptr<From, Deleter>&& p) {
    if (To* cast = dynamic_cast<To*>(p.get())) {
        std::unique_ptr<To, Deleter> result(cast, std::move(p.get_deleter()));
        p.release();
        return result;
    }
    return std::unique_ptr<To, Deleter>(
        nullptr);  // or throw std::bad_cast() if you prefer
}

}  // namespace SHV