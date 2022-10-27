#pragma once

#include "Material.hpp"

#include <glm/vec3.hpp>
#include <string>

namespace Engine {

class Render3DComponent {
  public:
    std::string model;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    bool instanced = false;
    bool updated = false;
    Material* material;

    Render3DComponent() {}
    Render3DComponent(const std::string& model, Material* material, float scale = 1.0f)
        : scale(scale), model(model), material(material) {}
};

} // namespace Engine