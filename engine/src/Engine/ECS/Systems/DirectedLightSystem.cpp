#include "DirectedLightSystem.hpp"

#include "DirectedLightComponent.hpp"
#include "LocationComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void DirectedLightSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto light = components.GetComponent<DirectedLightComponent>(entity).light;

        if (components.HasComponent<LocationComponent>(entity)) {
            const auto &location = components.GetComponent<LocationComponent>(entity);
            light.rotation = location.rotation;
        }

        scene.setDirectedLight(light);
    }
}

} // namespace Engine
