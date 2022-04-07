#pragma once

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "ParentComponent.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Engine {

class LocationComponent {
  public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    bool updated = false;
    bool prevUpdated = false;

    LocationComponent() {}

    LocationComponent(float x, float y, float z) : LocationComponent(glm::vec3(x, y, z)) {}

    LocationComponent(glm::vec3 position) : LocationComponent(position, glm::vec3(0)) {}

    LocationComponent(glm::vec3 position, glm::vec3 rotation)
        : position(position), rotation(rotation), front(Math::getDirection(rotation)) {}

    bool isUpdated(Entity entityId, ComponentManager &components) const {
        bool updated = this->updated;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            auto parentCmp = components.GetComponent<LocationComponent>(entityId);
            updated = updated || parentCmp.prevUpdated;
        }
        return updated;
    }

    static glm::mat4 getParentTransform(Entity entity, ComponentManager &components) {
        glm::mat4 worldTransform(1);

        Entity current = entity;
        while (components.HasComponent<ParentComponent>(current)) {
            current = components.GetComponent<ParentComponent>(current).entity;

            auto &location = components.GetComponent<LocationComponent>(current);

            worldTransform = glm::scale(glm::mat4(1.0), location.scale) * worldTransform;
            worldTransform = glm::toMat4(glm::quat(location.rotation)) * worldTransform;
            worldTransform = glm::translate(glm::mat4(1.0), location.position) * worldTransform;
        }

        return worldTransform;
    }

    static glm::mat4 getLocalTransform(Entity entity, ComponentManager &components) {
        auto &location = components.GetComponent<LocationComponent>(entity);

        glm::mat4 worldTransform(1);
        worldTransform = glm::scale(glm::mat4(1.0), location.scale) * worldTransform;
        worldTransform = glm::toMat4(glm::quat(location.rotation)) * worldTransform;
        worldTransform = glm::translate(glm::mat4(1.0), location.position) * worldTransform;

        return worldTransform;
    }

    static glm::mat4 getFullTransform(Entity entity, ComponentManager &components) {
        return getParentTransform(entity, components) * getLocalTransform(entity, components);
    }
};

} // namespace Engine