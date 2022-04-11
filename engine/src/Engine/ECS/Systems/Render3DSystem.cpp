#include "Render3DSystem.hpp"

#include "Application.hpp"
#include "Entity.hpp"

#include "cmath"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void Render3DSystem::OnRemoveComponent(Entity entity) const {
    auto &scene = getScene();
    auto &render = getCoordinator().GetComponent<Render3DComponent>(entity);

    if (!render.instanced) {
        return;
    }

    if (render.overlay()) {
        scene.removeOverlayObject(entity, render.model);
    } else {
        scene.removeObject(entity, render.model);
    }
    render.instanced = false;
}

void Render3DSystem::Attach(ComponentManager &components) const {
    const auto cmpArray = components.GetComponentArray<Render3DComponent>();
    cmpArray->beforeRemove$.addEventCallback([this](Entity entity) { OnRemoveComponent(entity); });
    cmpArray->active$.addEventCallback([this](Entity entity, bool isActive) {
        if (!isActive) {
            OnRemoveComponent(entity);
        }
    });
}

void Render3DSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        if (!render.instanced) {
            auto transform = LocationComponent::getFullTransform(entity, components) *
                             glm::scale(glm::mat4(1.0), render.scale) * glm::toMat4(glm::quat(render.rotation));

            if (render.overlay()) {
                scene.addOverlayObject(entity, render.model, transform, render.shader);
            } else {
                scene.addObject(entity, render.model, transform, render.shader);
            }
            render.instanced = true;
        } else if (location.isUpdated(entity, components) || render.isUpdated(entity, components)) {
            auto transform = LocationComponent::getFullTransform(entity, components) *
                             glm::scale(glm::mat4(1.0), render.scale) * glm::toMat4(glm::quat(render.rotation));

            if (render.overlay()) {
                scene.updateOverlayObject(entity, render.model, transform, render.shader);
            } else {
                scene.updateObject(entity, render.model, transform, render.shader);
            }

            location.prevUpdated = location.updated;
            location.updated = false;

            render.prevUpdated = render.updated;
            render.updated = false;
        }
    }
}

} // namespace Engine
