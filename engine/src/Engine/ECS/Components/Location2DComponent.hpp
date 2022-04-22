#pragma once

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "ParentComponent.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace Engine {

class Location2DComponent {
  public:
    typedef int TransformFlags;

    enum TransformFlags_ {
        TransformFlag_None = 0,
        TransformFlag_Translate = 1,
        TransformFlag_Rotate = 2,
        TransformFlag_Scale = 4
    };

    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 scale = glm::vec2(1.0f);
    float rotation = 0.0f;
    bool ndc = false;
    bool updated = false;

    Location2DComponent() {}

    Location2DComponent(float x, float y) : Location2DComponent(glm::vec2(x, y)) {}

    Location2DComponent(glm::vec2 position) : Location2DComponent(position, 0.0f) {}

    Location2DComponent(glm::vec2 position, float rotation) : position(position), rotation(rotation) {}

    bool isUpdated(Entity entityId, ComponentManager &components) const {
        bool updated = this->updated;
        while (components.HasComponent<ParentComponent>(entityId)) {
            entityId = components.GetComponent<ParentComponent>(entityId).entity;
            if (entityId == c_NoEntity) {
                break;
            }
            updated = updated || components.GetComponent<Location2DComponent>(entityId).updated;
        }
        return updated;
    }

    static bool hasNdc(Entity entityId, ComponentManager &components) {
        do {
            if (entityId == c_NoEntity) {
                break;
            }
            if (components.GetComponent<Location2DComponent>(entityId).ndc) {
                return true;
            }
        } while (components.HasComponent<ParentComponent>(entityId) &&
                 (entityId = components.GetComponent<ParentComponent>(entityId).entity));

        return false;
    }

    static glm::mat4 getParentTransform(Entity entity, ComponentManager &components, bool ndc = false) {
        glm::mat4 worldTransform(1);

        Entity current = entity;
        while (components.HasComponent<ParentComponent>(current)) {
            current = components.GetComponent<ParentComponent>(current).entity;
            if (current == c_NoEntity) {
                break;
            }

            auto &location = components.GetComponent<Location2DComponent>(current);
            if (location.ndc != ndc) {
                continue;
            }

            worldTransform = glm::scale(glm::mat4(1.0), glm::vec3(location.scale, 1.0f)) * worldTransform;
            worldTransform =
                glm::rotate(glm::mat4(1.0), location.rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * worldTransform;
            worldTransform = glm::translate(glm::mat4(1.0), glm::vec3(location.position, 0.0f)) * worldTransform;
        }

        return worldTransform;
    }

    static glm::mat4 getLocalTransform(Entity entity, ComponentManager &components, bool ndc = false) {
        auto &location = components.GetComponent<Location2DComponent>(entity);

        glm::mat4 worldTransform(1);
        if (location.ndc != ndc) {
            return worldTransform;
        }

        worldTransform = glm::scale(glm::mat4(1.0), glm::vec3(location.scale, 1.0f)) * worldTransform;
        worldTransform = glm::rotate(glm::mat4(1.0), location.rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * worldTransform;
        worldTransform = glm::translate(glm::mat4(1.0), glm::vec3(location.position, 0.0f)) * worldTransform;

        return worldTransform;
    }

    static glm::mat4 getNdcFix(Entity entity, ComponentManager &components) {
        return hasNdc(entity, components) ? glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f))
                                          : glm::mat4(1.0f);
    }

    static glm::mat4 getFullTransform(Entity entity, ComponentManager &components, bool ndc = false) {
        auto transform = getParentTransform(entity, components, ndc) * getLocalTransform(entity, components, ndc);
        if (ndc) {
            return transform * getNdcFix(entity, components);
        }
        return transform;
    }
};

} // namespace Engine