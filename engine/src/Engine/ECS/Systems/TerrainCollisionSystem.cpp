#include "TerrainCollisionSystem.hpp"
#include "Application.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "StaticRender3DComponent.hpp"
#include "TerrainCollisionComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace Engine {

void TerrainCollisionSystem::Update(ComponentManager &components) const {
    auto &coordinator = getCoordinator();
    auto &collision3D = getCollision();
    auto &models = getModels();

    std::vector<Entity> entitis(m_Entities.begin(), m_Entities.end());

    for (const auto &entity : entitis) {
        auto &location = components.GetComponent<LocationComponent>(entity);
        auto &render = components.GetComponent<StaticRender3DComponent>(entity);
        auto &terrain =
            components.GetComponent<TerrainCollisionComponent>(entity);
        auto renderModel = models.GetModel<InstancedModel>(render.model);

        std::vector<glm::vec3> vertices;
        for (auto &mesh : renderModel->meshes) {
            for (auto &vertex : mesh.vertices) {
                vertices.push_back(vertex.position * render.scale +
                                   location.position);
            }
        }

        collision3D.AddTerrain(entity, vertices, terrain.width, terrain.height);

        coordinator.RemoveComponent<TerrainCollisionComponent>(entity);
    }
}

} // namespace Engine
