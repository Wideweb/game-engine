#include "CameraSystem.hpp"
#include "Application.hpp"
#include "CameraComponent.hpp"
#include "LocationComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

void CameraSystem::Update(ComponentManager &components) const {
    auto &camera = Application::get().getCamera();

    for (const auto entity : m_Entities) {
        auto &location = components.GetComponent<LocationComponent>(entity);
        auto &offset = components.GetComponent<CameraComponent>(entity).offset;

        camera.setPosition(location.position);
        camera.setRotation(location.rotation);
        camera.move(offset);
    }
}

} // namespace Engine
