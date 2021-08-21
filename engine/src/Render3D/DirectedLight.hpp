#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Engine {

class DirectedLight {
  public:
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

    float nearPlane = 1.0f;
    float farPlane = 50.0f;

    bool operator==(const DirectedLight &other) {
        return ambient == other.ambient && diffuse == other.diffuse && specular == other.specular &&
               direction == other.direction && nearPlane == other.nearPlane && farPlane == other.farPlane;
    }

    bool operator!=(const DirectedLight &other) { return !(*this == other); }
};

} // namespace Engine
