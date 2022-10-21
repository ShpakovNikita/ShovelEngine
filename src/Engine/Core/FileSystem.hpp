#pragma once

#include <string>

namespace SHV {
class FileSystem {
   public:
    FileSystem() = default;
    virtual ~FileSystem() = default;

    virtual std::string GetPath(const std::string& relativeFilePath) const = 0;
};

class FileSystemImpl : public FileSystem {
   public:
    FileSystemImpl() = default;
    virtual ~FileSystemImpl() = default;

    virtual std::string GetPath(const std::string& relativeFilePath) const;
};
}  // namespace SHV