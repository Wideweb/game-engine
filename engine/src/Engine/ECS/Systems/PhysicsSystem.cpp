#include "PhysicsSystem.hpp"
#include "Application.hpp"
#include "PhysicsComponent.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

void PhysicsSystem::Update(ComponentManager &) const {
    // for (const auto entity : m_Entities) {
    // auto &physics = components.GetComponent<PhysicsComponent>(entity);
    // auto &velocity = components.GetComponent<VelocityComponent>(entity);

    // velocity.velocity.y -= physics.weight;
    // }
}

} // namespace Engine
