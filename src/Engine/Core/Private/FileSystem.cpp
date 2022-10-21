#include "Engine/Core/FileSystem.hpp"

#include "Engine/Common/Constants.hpp"

using namespace SHV;

std::string FileSystemImpl::GetPath(const std::string& relativeFilePath) const {
    return kAssetsDir + std::string("/") + relativeFilePath;
}