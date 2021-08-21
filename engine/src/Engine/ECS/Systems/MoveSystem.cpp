#include "MoveSystem.hpp"
#include "Application.hpp"
#include "LocationComponent.hpp"
#include "Math.hpp"
#include "VelocityComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

namespace Engine {

void MoveSystem::Update(ComponentManager &components) const {
    for (const auto entity : m_Entities) {
        auto &velocity = components.GetComponent<VelocityComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);
        float deltaTime = static_cast<float>(Application::get().getTime().getDeltaSeconds());

        location.rotation += velocity.rotation * deltaTime;
        location.front = glm::quat(location.rotation) * glm::vec3(0.0f, 0.0f, -1.0f);
        location.position += location.front * velocity.speed * deltaTime;
        location.position += velocity.velocity * deltaTime;

        // if (location.parent != c_NoEntity) {
        //     auto &otherLocation = components.GetComponent<LocationComponent>(location.parent);
        //     location.rotation = otherLocation.rotation;
        // }

        location.updated = true;
    }
}

} // namespace Engine
