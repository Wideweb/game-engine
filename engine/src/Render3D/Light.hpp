#pragma once

#include "CubeMap.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace Engine {

struct Light {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float constant = 1.0f;
    float linear = 0.01f;
    float quadratic = 0.008f;
};

} // namespace Engine
