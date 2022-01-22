#include "TerrainCollisionSystem.hpp"
#include "Application.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "Render3DComponent.hpp"
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
        auto &terrain = components.GetComponent<TerrainCollisionComponent>(entity);

        if (terrain.added && !terrain.updated) {
            continue;
        }

        auto &location = components.GetComponent<LocationComponent>(entity);
        // auto &render = components.GetComponent<StaticRender3DComponent>(entity);
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto renderModel = models.GetModel<InstancedModel>(render.model);

        auto model = GetTransform(entity, components, render, location);

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

glm::mat4x4 TerrainCollisionSystem::GetTransform(Entity entity, ComponentManager &components,
                                                 const Render3DComponent &render,
                                                 const LocationComponent location) const {
    glm::mat4 worldTransform(1);
    worldTransform = glm::translate(worldTransform, location.getFullPosition(entity, components));
    worldTransform = worldTransform *
                     glm::toMat4(glm::quat(location.getFullRotation(entity, components)) * glm::quat(render.rotation));
    worldTransform = glm::scale(worldTransform, render.getFullScale(entity, components));

    return worldTransform;
}

} // namespace Engine
