#include "CameraSystem.hpp"
#include "Application.hpp"
#include "CameraComponent.hpp"
#include "LocationComponent.hpp"

#include <glm/gtx/matrix_decompose.hpp>
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

        auto model = LocationComponent::getFullTransform(entity, components) *
                     glm::translate(glm::mat4(1.0), cameraCmp.offset) * glm::toMat4(glm::quat(cameraCmp.rotation));

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model, scale, rotation, position, skew, perspective);

        camera.setPosition(position);
        camera.setRotation(rotation);
        // camera.move(cameraCmp.offset);
    }
}

} // namespace Engine
