#include "MoveSystem.hpp"
#include "Application.hpp"
#include "LocationComponent.hpp"
#include "Math.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

void MoveSystem::Update(ComponentManager &components) const {
    for (const auto entity : m_Entities) {
        auto &velocity = components.GetComponent<VelocityComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        location.rotation += velocity.rotation;
        location.front = Math::getDirection(location.rotation);
        location.position += location.front * velocity.speed;
        location.position += velocity.velocity;

        // if (location.parent != c_NoEntity) {
        //     auto &otherLocation = components.GetComponent<LocationComponent>(location.parent);
        //     location.rotation = otherLocation.rotation;
        // }

        location.updated = true;
    }
}

} // namespace Engine
