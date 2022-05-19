#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Engine {

class SpotLight {
  public:
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float intensity = 1.0f;

    float constant = 1.0f;
    float linear = 0.01f;
    float quadratic = 0.008f;

    // Light Space Perspective Projection
    float shadowFrustumNearPlane = 0.01f;
    float shadowFrustumFarPlane = 20.0f;
    int shadowFrustumWidth = 1024;
    int shadowFrustumHeight = 1024;

    // Shadow Settings
    double bias = 0.15f;
    double pcfDiskRadius = 0.02f;
    int pcfSamples = 20;

    bool operator==(const SpotLight &other) {
        return ambient == other.ambient && diffuse == other.diffuse && specular == other.specular &&
               intensity == other.intensity && shadowFrustumNearPlane == other.shadowFrustumNearPlane &&
               shadowFrustumFarPlane == other.shadowFrustumFarPlane && shadowFrustumWidth == other.shadowFrustumWidth &&
               shadowFrustumHeight == other.shadowFrustumHeight && bias == other.bias && constant == other.constant &&
               linear == other.linear && quadratic == other.quadratic && pcfDiskRadius == other.pcfDiskRadius &&
               pcfSamples == other.pcfSamples;
    }

    bool operator!=(const SpotLight &other) { return !(*this == other); }
};

} // namespace Engine
