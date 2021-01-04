#include "CollisionSystem.hpp"
#include "Application.hpp"
#include "CollisionComponent.hpp"
#include "LocationComponent.hpp"
#include "VelocityComponent.hpp"

#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

namespace Engine {

const void CollisionSystem::Update(ComponentManager &components) {
    auto &app = Application::get();
    auto &eventHandler = app.getEventHandler();

    std::vector<CollisionShape> colliders;
    for (const auto entity : m_Entities) {
        auto &collision = components.GetComponent<CollisionComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);
        auto &velocity = components.GetComponent<VelocityComponent>(entity);

        collision.entities.clear();

        glm::vec2 move(location.position);
        move = move + glm::vec2(location.front) * glm::vec2(velocity.speed);

        std::vector<glm::vec2> vertices;
        std::transform(collision.vertices.begin(), collision.vertices.end(),
                       std::back_inserter(vertices),
                       [&](glm::vec2 &v) { return v + move; });

        colliders.emplace_back(entity, vertices, collision.isStatic);
    }

    if (colliders.empty()) {
        return;
    }

    std::vector<CollisionResult> results =
        m_CollisionDetection.detect(colliders);

    for (auto &result : results) {
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

        location.position.y += result.mtv.y;
        location.position.x += result.mtv.x;

        if (result.mtv.x != 0) {
            // location.position.x += velocity.velocity.x;
            // velocity.velocity.x = 0;
        }

        if (result.mtv.y != 0) {
            // location.position.y += velocity.velocity.y;
            // velocity.velocity.y = 0;
        }

        BeginCollisionEvent event;
        event.first = result.shape1;
        event.second = result.shape2;
        eventHandler.send<BeginCollisionEvent>(event);
    }
}

} // namespace Engine
