#include "PhysicsSystem.hpp"
#include "Application.hpp"
#include "PhysicsComponent.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

void PhysicsSystem::Update(ComponentManager &components) const {
    for (const auto entity : m_Entities) {
        auto &physics = components.GetComponent<PhysicsComponent>(entity);
        auto &velocity = components.GetComponent<VelocityComponent>(entity);
        float deltaTime = static_cast<float>(Application::get().getTime().getDeltaSeconds());

        velocity.velocity.y -= physics.weight * deltaTime;
    }
}

} // namespace Engine
