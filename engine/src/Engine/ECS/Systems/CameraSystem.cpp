#include "CameraSystem.hpp"
#include "Application.hpp"
#include "CameraComponent.hpp"
#include "LocationComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

namespace Engine {

void CameraSystem::Update(ComponentManager &components) const {
    if (Application::get().getTime().poused()) {
        return;
    }

    auto &camera = Application::get().getCamera();

    for (const auto entity : m_Entities) {
        auto &location = components.GetComponent<LocationComponent>(entity);
        auto &cameraCmp = components.GetComponent<CameraComponent>(entity);

        camera.setPosition(location.position + glm::quat(location.rotation) * cameraCmp.offset);
        camera.setRotation(location.rotation + cameraCmp.rotation);
        // camera.move(cameraCmp.offset);
    }
}

} // namespace Engine
