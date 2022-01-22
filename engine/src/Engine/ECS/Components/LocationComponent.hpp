#pragma once

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "ParentComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

namespace Engine {

class LocationComponent {
  public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    bool updated = false;
    bool prevUpdated = false;

    LocationComponent() {}

    LocationComponent(float x, float y, float z) : LocationComponent(glm::vec3(x, y, z)) {}

    LocationComponent(glm::vec3 position) : LocationComponent(position, glm::vec3(0)) {}

    LocationComponent(glm::vec3 position, glm::vec3 rotation)
        : position(position), rotation(rotation), front(Math::getDirection(rotation)) {}

    glm::vec3 getFullPosition(Entity entityId, ComponentManager &components) const {
        glm::vec3 position = this->position;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            auto parentCmp = components.GetComponent<LocationComponent>(entityId);
            position += parentCmp.position;
        }
        return position;
    }

    glm::vec3 getFullRotation(Entity entityId, ComponentManager &components) const {
        auto rotation = glm::quat(this->rotation);
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            auto parentCmp = components.GetComponent<LocationComponent>(entityId);
            rotation = glm::quat(parentCmp.rotation) * rotation;
        }
        return glm::eulerAngles(rotation);
    }

    bool isUpdated(Entity entityId, ComponentManager &components) const {
        bool updated = this->updated;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            auto parentCmp = components.GetComponent<LocationComponent>(entityId);
            updated = updated || parentCmp.prevUpdated;
        }
        return updated;
    }
};

} // namespace Engine