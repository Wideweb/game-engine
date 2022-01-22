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
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    bool instanced = false;
    bool updated = false;
    bool prevUpdated = false;
    std::shared_ptr<Shader> shader;

    Render3DComponent() {}
    Render3DComponent(std::string model, float scale, bool overlay = false)
        : scale(scale), model(std::move(model)), m_Overlay(overlay) {}

    bool overlay() const { return m_Overlay; }

    glm::vec3 getFullScale(Entity entityId, ComponentManager &components) const {
        auto scale = this->scale;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            auto parentCmp = components.GetComponent<Render3DComponent>(entityId);
            scale = scale * parentCmp.scale;
        }
        return scale;
    }

    bool isUpdated(Entity entityId, ComponentManager &components) const {
        bool updated = this->updated;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            auto parentCmp = components.GetComponent<Render3DComponent>(entityId);
            updated = updated || parentCmp.prevUpdated;
        }
        return updated;
    }
};

} // namespace Engine