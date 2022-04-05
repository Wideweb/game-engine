#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Engine {

class DirectedLight {
  public:
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    float intensity = 1.0f;

    float nearPlane = 1.0f;
    float farPlane = 25.0f;

    double biasFactor = 0.0f;
    double biasMin = 0.02f;
    int pcf = 4;

    bool operator==(const DirectedLight &other) {
        return ambient == other.ambient && diffuse == other.diffuse && specular == other.specular &&
               rotation == other.rotation && intensity == other.intensity && nearPlane == other.nearPlane &&
               farPlane == other.farPlane && biasFactor == other.biasFactor && biasMin == other.biasMin &&
               pcf == other.pcf;
    }

    bool operator!=(const DirectedLight &other) { return !(*this == other); }
};

} // namespace Engine
