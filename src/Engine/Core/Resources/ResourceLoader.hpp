#pragma once

#include <string>
#include <memory>

namespace SHV {
template <typename ResourceType>
class ResourceLoader {
   public:
    ResourceLoader() = default;
    virtual ~ResourceLoader() = default;
    
    virtual std::shared_ptr<ResourceType> Create(
        const std::string& resourcePath) = 0;
    virtual void Release(std::shared_ptr<ResourceType> resource) = 0;
};
}  // namespace SHV