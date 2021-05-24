#pragma once

#include <memory>

#include "Texture.hpp"

namespace Engine {

struct Material {
    std::shared_ptr<Texture> diffuseMap;
    std::shared_ptr<Texture> specularMap;
    std::shared_ptr<Texture> normalMap;
    float specular = 1.0f;
    float shininess = 128.0f;
};

} // namespace Engine
