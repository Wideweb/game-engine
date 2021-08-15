#include "Render3DSystem.hpp"

#include "Application.hpp"
#include "Entity.hpp"
#include "ModelInstanceManager.hpp"

#include "cmath"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void Render3DSystem::Attach(ComponentManager &components) const {
    components.GetComponentArray<Render3DComponent>()->beforeRemove$.addEventCallback([this](Entity entity) {
        auto &scene = getScene();
        auto &render = getCoordinator().GetComponent<Render3DComponent>(entity);

        if (render.overlay()) {
            scene.removeOverlayObject(render.model, render.instance);
        } else {
            scene.removeObject(render.model, render.instance);
        }
    });
}

void Render3DSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        if (render.instance == NoModelInstance) {
            auto transform = GetTransform(render, location);

            if (render.overlay()) {
                render.instance = scene.addOverlayObject(render.model, transform, entity);
            } else {
                render.instance = scene.addObject(render.model, transform, entity);
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
    glm::mat4 worldTransform(1);
    worldTransform = glm::translate(worldTransform, location.position);
    worldTransform = worldTransform * glm::toMat4(glm::quat(location.rotation) * glm::quat(render.rotation));
    worldTransform = glm::scale(worldTransform, glm::vec3(render.scale));

    return worldTransform;
}

} // namespace Engine
