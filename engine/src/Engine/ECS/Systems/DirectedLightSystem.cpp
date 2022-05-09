#include "DirectedLightSystem.hpp"

#include "DirectedLightComponent.hpp"
#include "LocationComponent.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void DirectedLightSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto light = components.GetComponent<DirectedLightComponent>(entity).light;
        auto model = LocationComponent::getFullTransform(entity, components);

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model, scale, rotation, position, skew, perspective);

        scene.setDirectedLight(light, position, rotation);
    }
}

} // namespace Engine
