#pragma once

#include <memory>

#include "Texture.hpp"

namespace Engine {

struct Material {
    Texture diffuseMap;
    Texture specularMap;
    Texture normalMap;
    float specular = 1.0f;
    float shininess = 128.0f;
};

} // namespace Engine
