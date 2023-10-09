#pragma once

#include <string>

namespace SHV::PbrParams {
// Texture maps
static std::string kAlbedoMap = "albedoMap";
static std::string kAoMap = "aoMap";
static std::string kHeightMap = "heightMap";
static std::string kMetallicRoughnessMap = "metallicRoughnessMap";
static std::string kNormalMap = "normalMap";
static std::string kEmissiveMap = "emissiveMap";

// Params
static std::string kRoughnessFactor = "roughnessFactor";
static std::string kMetallicFactor = "metallicFactor";
static std::string kBaseColorFactor = "baseColorFactor";
static std::string kEmissiveFactor = "emissiveFactor";
}  // namespace SHV::PbrParams

namespace SHV::SkyboxParams {
// Texture maps
static std::string kSkyboxMap = "skyboxMap";
}  // namespace SHV::SkyboxParams
