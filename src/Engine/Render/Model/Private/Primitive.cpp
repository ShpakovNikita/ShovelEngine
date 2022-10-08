#include "Engine/Render/Model/Primitive.hpp"

#include "Engine/Render/Model/Material.hpp"

using namespace SHV;

Primitive::Primitive() = default;

Primitive::Primitive(std::shared_ptr<Material> aMaterial)
    : material(aMaterial) {}
