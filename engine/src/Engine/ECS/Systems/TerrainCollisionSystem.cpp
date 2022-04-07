#include "TerrainCollisionSystem.hpp"
#include "Application.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "Render3DComponent.hpp"
#include "TerrainCollisionComponent.hpp"

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
        auto &terrain = components.GetComponent<TerrainCollisionComponent>(entity);

        if (terrain.added && !terrain.updated) {
            continue;
        }

        auto &location = components.GetComponent<LocationComponent>(entity);
        // auto &render = components.GetComponent<StaticRender3DComponent>(entity);
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto renderModel = models.GetModel<InstancedModel>(render.model);

        auto model = LocationComponent::getFullTransform(entity, components);

        std::vector<glm::vec3> vertices;
        for (auto &mesh : renderModel->meshes) {
            for (auto &vertex : mesh.vertices) {
                vertices.push_back(glm::vec3(model * glm::vec4(vertex.position, 1.0f)));
            }
        }

        if (!terrain.added) {
            collision3D.AddTerrain(entity, vertices, terrain.columns, terrain.rows);
            terrain.added = true;
        } else {
            collision3D.UpdateTerrain(entity, vertices, terrain.columns, terrain.rows);
        }

        terrain.updated = false;

        // coordinator.RemoveComponent<TerrainCollisionComponent>(entity);
    }
}

} // namespace Engine
