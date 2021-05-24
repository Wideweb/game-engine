#include "DirectedLightSystem.hpp"

#include "DirectedLightComponent.hpp"

namespace Engine {

void DirectedLightSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        const auto &light = components.GetComponent<DirectedLightComponent>(entity);

        scene.setDirectedLight(light.light);
    }
}

} // namespace Engine
