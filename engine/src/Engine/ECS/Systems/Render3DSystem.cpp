#include "Render3DSystem.hpp"

#include "Application.hpp"
#include "ModelInstanceManager.hpp"
#include "cmath"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

void Render3DSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        if (render.instance == NoModelInstance) {
            auto transform = GetTransform(render, location);

            if (render.overlay()) {
                render.instance = scene.addOverlayObject(render.model, transform);
            } else {
                render.instance = scene.addObject(render.model, transform);
            }

        } else if (location.updated || render.updated) {
            auto transform = GetTransform(render, location);

            if (render.overlay()) {
                scene.updateOverlayObject(render.model, transform, render.instance);
            } else {
                scene.updateObject(render.model, transform, render.instance);
            }

            location.updated = false;
            render.updated = false;
        }
    }
}

glm::mat4x4 Render3DSystem::GetTransform(const Render3DComponent &render, const LocationComponent location) const {
    glm::mat4 modelTransform(1);
    modelTransform = glm::rotate(modelTransform, render.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelTransform = glm::rotate(modelTransform, render.rotation.y, glm::vec3(0.0f, -1.0f, 0.0f));
    modelTransform = glm::rotate(modelTransform, render.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 worldTransform(1);
    worldTransform = glm::translate(worldTransform, location.position);
    worldTransform = glm::rotate(worldTransform, location.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    worldTransform = glm::rotate(worldTransform, location.rotation.y, glm::vec3(0.0f, -1.0f, 0.0f));
    worldTransform = glm::rotate(worldTransform, location.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    worldTransform = glm::scale(worldTransform, glm::vec3(render.scale));

    return worldTransform * modelTransform;
}

} // namespace Engine
