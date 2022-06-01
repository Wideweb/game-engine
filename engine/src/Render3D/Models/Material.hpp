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

    // PBR
    Texture metallicMap;
    Texture roughnessMap;
    Texture ambientOcclusionMap;

    bool empty() const {
        return diffuseMap.empty() && specularMap.empty() && normalMap.empty() && metallicMap.empty() &&
               roughnessMap.empty() && ambientOcclusionMap.empty();
    }
};

} // namespace Engine
