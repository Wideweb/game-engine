#include "StaticCollisionSystem.hpp"
#include "Application.hpp"
#include "LocationComponent.hpp"
#include "StaticCollisionComponent.hpp"

#include <glm/vec2.hpp>
#include <vector>

namespace Engine {

void StaticCollisionSystem::Update(ComponentManager &components) const {
    // auto &coordinator = getCoordinator();
    auto &collision3D = getCollision();

    std::vector<glm::vec3> vertices;
    vertices.reserve(8);

    std::vector<Entity> entitis(m_Entities.begin(), m_Entities.end());

    for (const auto &entity : entitis) {
        auto &collision = components.GetComponent<StaticCollisionComponent>(entity);

        if (collision.added && !collision.updated) {
            continue;
        }

        auto &location = components.GetComponent<LocationComponent>(entity);

        vertices.clear();

        std::transform(collision.vertices.begin(), collision.vertices.end(), std::back_inserter(vertices),
                       [&](const glm::vec3 &v) { return v + location.position; });

        if (!collision.added) {
            collision3D.AddShape(entity, vertices);
        } else {
            collision3D.UpdateShape(entity, vertices);
        }

        collision.updated = false;
        // coordinator.RemoveComponent<StaticCollisionComponent>(entity);
    }
}

} // namespace Engine
