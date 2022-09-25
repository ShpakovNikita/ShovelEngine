#pragma once

#ifdef SHV_DEBUG
    constexpr bool IsDebugBuild = true;
#else
    constexpr bool IsDebugBuild = false;
#endif
