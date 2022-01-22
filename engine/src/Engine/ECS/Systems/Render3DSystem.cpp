#include "Render3DSystem.hpp"

#include "Application.hpp"
#include "Entity.hpp"

#include "cmath"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void Render3DSystem::Attach(ComponentManager &components) const {
    components.GetComponentArray<Render3DComponent>()->beforeRemove$.addEventCallback([this](Entity entity) {
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
    });
}

void Render3DSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        if (!render.instanced) {
            auto transform = GetTransform(entity, components, render, location);

            if (render.overlay()) {
                scene.addOverlayObject(entity, render.model, transform, render.shader);
            } else {
                scene.addObject(entity, render.model, transform, render.shader);
            }
            render.instanced = true;
        } else if (location.isUpdated(entity, components) || render.isUpdated(entity, components)) {
            auto transform = GetTransform(entity, components, render, location);

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

glm::mat4x4 Render3DSystem::GetTransform(Entity entity, ComponentManager &components, const Render3DComponent &render,
                                         const LocationComponent location) const {
    glm::mat4 worldTransform(1);

    std::vector<Entity> ancestors;
    Entity current = entity;
    while (components.HasComponent<ParentComponent>(current)) {
        current = components.GetComponent<ParentComponent>(current).entity;
        ancestors.push_back(current);
    }

    for (int i = ancestors.size() - 1; i >= 0; i--) {
        Entity parent = ancestors[i];
        auto parentLocation = components.GetComponent<LocationComponent>(parent);
        auto parentRender = components.GetComponent<Render3DComponent>(parent);

        worldTransform = glm::translate(worldTransform, parentLocation.position);
        worldTransform = worldTransform * glm::toMat4(glm::quat(parentLocation.rotation));
        worldTransform = glm::scale(worldTransform, parentRender.scale);
    }

    worldTransform = glm::translate(worldTransform, location.position);
    worldTransform = worldTransform * glm::toMat4(glm::quat(location.rotation) * glm::quat(render.rotation));
    worldTransform = glm::scale(worldTransform, render.scale);

    return worldTransform;
}

} // namespace Engine
