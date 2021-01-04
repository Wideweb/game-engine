#include "MoveSystem.hpp"
#include "Application.hpp"
#include "LocationComponent.hpp"
#include "Math.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

const void MoveSystem::Update(ComponentManager &components) {
    auto &app = Application::get();

    for (const auto entity : m_Entities) {
        auto &velocity = components.GetComponent<VelocityComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        location.rotation += velocity.rotation;
        location.front = Math::getDirection(location.rotation);
        location.position += location.front * velocity.speed;
    }
}

} // namespace Engine
