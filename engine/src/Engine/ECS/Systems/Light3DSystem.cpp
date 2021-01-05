#include "Light3DSystem.hpp"

#include "Application.hpp"
#include "Light3DComponent.hpp"
#include "LocationComponent.hpp"
#include "cmath"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

void Light3DSystem::Update(ComponentManager &components) const {
    auto &scene = Application::get().getScene();

    for (auto entity : m_Entities) {
        const auto &light = components.GetComponent<Light3DComponent>(entity);
        const auto &location =
            components.GetComponent<LocationComponent>(entity);

        scene.addLight(light.light, location.position);
    }
}

} // namespace Engine
