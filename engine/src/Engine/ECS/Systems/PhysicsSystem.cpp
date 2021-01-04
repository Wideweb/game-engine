#include "PhysicsSystem.hpp"
#include "Application.hpp"
#include "PhysicsComponent.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

const void PhysicsSystem::Update(ComponentManager &components) {
    auto &app = Application::get();

    for (const auto entity : m_Entities) {
        auto &physics = components.GetComponent<PhysicsComponent>(entity);
        auto &velocity = components.GetComponent<VelocityComponent>(entity);

        // velocity.velocity.y -= physics.weight;
    }
}

} // namespace Engine
