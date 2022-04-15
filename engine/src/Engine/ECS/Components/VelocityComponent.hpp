#pragma once

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "ParentComponent.hpp"
#include "RenderSettings.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class VelocityComponent {
  public:
    float speed = 0.0f;
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);

    VelocityComponent() {}
    VelocityComponent(float speed, glm::vec3 rotation, glm::vec3 velocity)
        : speed(speed), rotation(std::move(rotation)), velocity(std::move(velocity)) {}

    glm::vec3 getFullVelocity(Entity entityId, ComponentManager &components) const {
        auto velocity = this->velocity;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            if (entityId == c_NoEntity) {
                break;
            }
            auto parentCmp = components.GetComponent<VelocityComponent>(entityId);
            velocity += parentCmp.velocity;
        }
        return velocity;
    }

    glm::vec3 getFullRotation(Entity entityId, ComponentManager &components) const {
        auto rotation = glm::quat(this->rotation);
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            if (entityId == c_NoEntity) {
                break;
            }
            auto parentCmp = components.GetComponent<VelocityComponent>(entityId);
            rotation = glm::quat(parentCmp.rotation) * rotation;
        }
        return glm::eulerAngles(rotation);
    }
};

} // namespace Engine