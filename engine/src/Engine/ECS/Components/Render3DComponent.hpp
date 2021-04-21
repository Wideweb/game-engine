#pragma once

#include "Entity.hpp"
#include "ModelInstanceManager.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <glm/vec3.hpp>
#include <string>
#include <utility>
#include <vector>

namespace Engine {

class Render3DComponent {
  public:
    float scale;
    std::string model;
    glm::vec3 rotation = glm::vec3(0.0f);
    ModelInstance instance = NoModelInstance;

    Render3DComponent() {}
    Render3DComponent(std::string model, float scale)
        : scale(scale), model(std::move(model)) {}
};

} // namespace Engine