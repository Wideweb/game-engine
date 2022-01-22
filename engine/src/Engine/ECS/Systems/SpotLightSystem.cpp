#include "SpotLightSystem.hpp"

#include "LocationComponent.hpp"
#include "SpotLightComponent.hpp"

namespace Engine {

void SpotLightSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        const auto &light = components.GetComponent<SpotLightComponent>(entity);
        const auto &location = components.GetComponent<LocationComponent>(entity);

        scene.addSpotLight(entity, light.light, location.position);
    }
}

} // namespace Engine
