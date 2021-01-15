#include "StaticCollisionSystem.hpp"
#include "Application.hpp"
#include "LocationComponent.hpp"
#include "StaticCollisionComponent.hpp"

#include <glm/vec2.hpp>

namespace Engine {

void StaticCollisionSystem::Update(ComponentManager &components) {
    auto &app = Application::get();
    auto &collision3D = app.getCollision();

    std::vector<glm::vec3> vertices;
    vertices.reserve(8);

    for (const auto entity : m_Entities) {
        auto &collision =
            components.GetComponent<StaticCollisionComponent>(entity);

        if (collision.created) {
            continue;
        }

        auto &location = components.GetComponent<LocationComponent>(entity);

        vertices.clear();

        std::transform(collision.vertices.begin(), collision.vertices.end(),
                       std::back_inserter(vertices), [&](const glm::vec3 &v) {
                           return v + location.position;
                       });

        collision3D.AddShape(entity, vertices);
        collision.created = true;
    }
}

} // namespace Engine
