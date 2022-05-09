#pragma once

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "ParentComponent.hpp"
#include "RenderSettings.hpp"
#include "Shader.hpp"

#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Engine {

class Render3DComponent {
  private:
    bool m_Overlay;

  public:
    std::string model;
    std::string prevModel;
    bool modelChanged = false;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    bool instanced = false;
    bool updated = false;
    std::shared_ptr<Shader> shader;

    Render3DComponent() {}
    Render3DComponent(std::string model, float scale = 1.0f, bool overlay = false)
        : scale(scale), model(std::move(model)), m_Overlay(overlay) {}

    bool overlay() const { return m_Overlay; }

    void setModel(const std::string &model) {
        if (this->model == model) {
            return;
        }
        this->prevModel = this->model;
        this->model = model;
        modelChanged = true;
    }
};

} // namespace Engine