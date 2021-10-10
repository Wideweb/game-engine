#pragma once

#include "Entity.hpp"
#include "ModelInstanceManager.hpp"

#include <glm/vec3.hpp>
#include <string>
#include <utility>
#include <vector>

namespace Engine {

class Render3DComponent {
  private:
    bool m_Overlay;

  public:
    std::string model;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    ModelInstance instance = c_NoModelInstance;
    bool updated = false;

    Render3DComponent() {}
    Render3DComponent(std::string model, float scale, bool overlay = false)
        : scale(scale), model(std::move(model)), m_Overlay(overlay) {}

    bool overlay() const { return m_Overlay; }
};

} // namespace Engine