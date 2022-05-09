#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Engine {

struct DirectedLight {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float intensity = 1.0f;

    // Light Space Orthographic Projection
    float shadowFrustumNearPlane = 0.01f;
    float shadowFrustumFarPlane = 20.0f;
    float shadowFrustumWidth = 20.0f;
    float shadowFrustumHeight = 20.0f;

    // Depth Map
    glm::vec2 depthMap = glm::vec2(960.0f, 540.0f);
    bool depthMapRelativeToViewport = true;
    float depthMapToViewportFactor = 2.0f;

    // Shadow Settings
    double biasFactor = 0.0f;
    double biasMin = 0.01f;
    int pcf = 4;

    bool operator==(const DirectedLight &other) {
        return ambient == other.ambient && diffuse == other.diffuse && specular == other.specular &&
               intensity == other.intensity && shadowFrustumNearPlane == other.shadowFrustumNearPlane &&
               shadowFrustumFarPlane == other.shadowFrustumFarPlane && shadowFrustumWidth == other.shadowFrustumWidth &&
               shadowFrustumHeight == other.shadowFrustumHeight && depthMap == other.depthMap &&
               depthMapRelativeToViewport == other.depthMapRelativeToViewport &&
               depthMapToViewportFactor == other.depthMapToViewportFactor && biasFactor == other.biasFactor &&
               biasMin == other.biasMin && pcf == other.pcf;
    }

    bool operator!=(const DirectedLight &other) { return !(*this == other); }
};

} // namespace Engine
