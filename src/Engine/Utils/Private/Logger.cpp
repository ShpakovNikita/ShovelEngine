#include "Engine/Utils/Logger.hpp"
#include "Engine/Common/Constants.hpp"

#include <map>

namespace SLogger
{
    static std::ostream kNullOstream(nullptr);

    std::string BuildPrefix(const std::string& appendix, SHV::eTag tag)
    {
        static const std::map<SHV::eTag, std::string> TagNames =
        {
            { SHV::eTag::kBase,         "Base"         },
            { SHV::eTag::kRender,       "Render"       },
            { SHV::eTag::kAssetLoading, "AssetLoading" },
            { SHV::eTag::kEngine,       "Engine"       },
        };
        
        return appendix + std::string(" [") + TagNames.find(tag)->second + std::string("]: ");
    }
}

std::ostream& SHV::LogD(eTag tag)
{
    if constexpr (IsDebugBuild)
    {
        return std::cout << SLogger::BuildPrefix("(Debug)", tag);
    }
    
    return SLogger::kNullOstream;
}

std::ostream& SHV::LogI(eTag tag)
{
    return std::cout << SLogger::BuildPrefix("(Info)", tag);
}

std::ostream& SHV::LogW(eTag tag)
{
    return std::cerr << SLogger::BuildPrefix("(Warning)", tag);
}

std::ostream& SHV::LogE(eTag tag)
{
    return std::cerr << SLogger::BuildPrefix("(Error)", tag);
}
