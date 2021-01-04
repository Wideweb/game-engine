#include "CameraSystem.hpp"
#include "Application.hpp"
#include "LocationComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

const void CameraSystem::Update(ComponentManager &components) {
    auto &app = Application::get();

    for (const auto entity : m_Entities) {
        auto &location = components.GetComponent<LocationComponent>(entity);

        app.getCamera().setPosition(location.position);
        app.getCamera().setRotation(location.rotation);
    }
}

} // namespace Engine
