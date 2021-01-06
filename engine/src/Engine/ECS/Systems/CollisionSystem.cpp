#include "CollisionSystem.hpp"
#include "Application.hpp"
#include "CollisionComponent.hpp"
#include "LocationComponent.hpp"
#include "Math.hpp"
#include "VelocityComponent.hpp"

#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

namespace Engine {

void CollisionSystem::Update(ComponentManager &components) const {
    auto &app = Application::get();
    auto &eventHandler = app.getEventHandler();

    std::vector<CollisionShape> colliders;
    colliders.reserve(m_Entities.size());

    std::vector<glm::vec3> vertices;
    vertices.reserve(8);

    for (const auto entity : m_Entities) {
        auto &collision = components.GetComponent<CollisionComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);
        auto &velocity = components.GetComponent<VelocityComponent>(entity);

        collision.entities.clear();
        vertices.clear();

        glm::vec3 move(location.position);
        move = move + glm::vec3(location.front) * glm::vec3(velocity.speed);

        std::transform(collision.vertices.begin(), collision.vertices.end(),
                       std::back_inserter(vertices),
                       [&](const glm::vec3 &v) { return v + move; });

        colliders.emplace_back(entity, vertices, collision.isStatic);
    }

    if (colliders.empty()) {
        return;
    }

    std::vector<CollisionResult> results =
        m_CollisionDetection.detect(colliders);

    for (const auto &result : results) {
        auto &location =
            components.GetComponent<LocationComponent>(result.shape1);
        auto &velocity =
            components.GetComponent<VelocityComponent>(result.shape1);
        auto &collision1 =
            components.GetComponent<CollisionComponent>(result.shape1);
        auto &collision2 =
            components.GetComponent<CollisionComponent>(result.shape2);

        collision1.entities.insert(result.shape2);
        collision2.entities.insert(result.shape1);

        location.position += result.mtv;
        velocity.speed = 0;

        if (!Math::isEqual(result.mtv.x, 0.0f)) {
            velocity.velocity.x = 0;
        }

        if (!Math::isEqual(result.mtv.y, 0.0f)) {
            velocity.velocity.y = 0;
        }

        if (!Math::isEqual(result.mtv.z, 0.0f)) {
            velocity.velocity.z = 0;
        }

        BeginCollisionEvent event;
        event.first = result.shape1;
        event.second = result.shape2;
        eventHandler.send<BeginCollisionEvent>(event);
    }
}

} // namespace Engine
