#include "SpotLightSystem.hpp"

#include "LocationComponent.hpp"
#include "SpotLightComponent.hpp"

namespace Engine {

void SpotLightSystem::OnRemoveComponent(Entity entity) const {
    auto &light = getCoordinator().GetComponent<SpotLightComponent>(entity);

    if (!light.instanced) {
        return;
    }

    getScene().removeSpotLight(entity);
    light.instanced = false;
}

void SpotLightSystem::Attach(ComponentManager &components) const {
    const auto cmpArray = components.GetComponentArray<SpotLightComponent>();
    cmpArray->beforeRemove$.addEventCallback([this](Entity entity) { OnRemoveComponent(entity); });
    cmpArray->active$.addEventCallback([this](Entity entity, bool isActive) {
        if (!isActive) {
            OnRemoveComponent(entity);
        }
    });
}

void SpotLightSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto model = LocationComponent::getFullTransform(entity, components);

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model, scale, rotation, position, skew, perspective);

        auto &light = components.GetComponent<SpotLightComponent>(entity);
        if (!light.instanced) {
            scene.addSpotLight(entity, light.light, position);
            light.instanced = true;
        } else {
            scene.updateSpotLight(entity, light.light, position);
        }
    }
}

} // namespace Engine
