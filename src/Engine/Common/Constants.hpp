#pragma once

#ifdef SHV_DEBUG
constexpr bool IsDebugBuild = true;
#else
constexpr bool IsDebugBuild = false;
#endif

#ifdef ASSETS_DIR
constexpr const char* kAssetsDir = ASSETS_DIR;
#else
constexpr const char* kAssetsDir = ".";
#endif