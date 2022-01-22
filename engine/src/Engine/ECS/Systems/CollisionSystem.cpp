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
    auto &collision3D = getCollision();
    auto &eventHandler = app.getEventHandler();

    std::vector<glm::vec3> vertices;
    vertices.reserve(8);

    for (const auto entity : m_Entities) {
        auto &collision = components.GetComponent<CollisionComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);
        auto &velocity = components.GetComponent<VelocityComponent>(entity);
        float deltaTime = static_cast<float>(Application::get().getTime().getDeltaSeconds());

        vertices.clear();

        glm::vec3 move(location.getFullPosition(entity, components));
        move = move + glm::vec3(location.front) * glm::vec3(velocity.speed) * deltaTime;

        std::transform(collision.vertices.begin(), collision.vertices.end(), std::back_inserter(vertices),
                       [&](const glm::vec3 &v) { return v + move; });

        auto results = collision3D.Detect(vertices);

        collision.entities.clear();
        for (const auto &result : results) {

            if (glm::dot(result.mtv, glm::vec3(0.0f, 1.0f, 0.0f)) > 0.0f) {
                collision.entities.insert(result.id);
            }

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
            event.first = entity;
            event.second = result.id;
            eventHandler.send<BeginCollisionEvent>(event);
        }
    }
}

} // namespace Engine
