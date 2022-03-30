#pragma once

#include "CollisionDetection.hpp"
#include "CollisionShape.hpp"
#include "Grid.hpp"
#include "Math.hpp"

#include <cmath>
#include <memory>
#include <optional>
#include <utility>

namespace Engine {

template <typename T> class Collision3D {
  public:
    Collision3D() {
        AABB gridBox(glm::vec3(-100.0f, -100.0f, -100.0f), glm::vec3(100.0f, 100.0f, 100.0f));
        m_Grid = std::make_unique<Grid<T>>(gridBox);
    }

    void AddTerrain(T id, const std::vector<glm::vec3> &vertices, unsigned int columns, unsigned int rows) {
        CollisionShape<T> shape(id, vertices, CollisionShapeType::Terrain);
        shape.columns = columns;
        shape.rows = rows;
        m_Grid->AddShape(shape);
    }

    void UpdateTerrain(T id, const std::vector<glm::vec3> &vertices, unsigned int columns, unsigned int rows) {
        CollisionShape<T> shape(id, vertices, CollisionShapeType::Terrain);
        DestroyShape(id);
        AddTerrain(id, vertices, columns, rows);
    }

    void AddShape(T id, const std::vector<glm::vec3> &vertices) {
        CollisionShape<T> shape(id, vertices, CollisionShapeType::Object);
        m_Grid->AddShape(shape);
    }

    void UpdateShape(T id, const std::vector<glm::vec3> &vertices) { m_Grid->UpdateShape(id, vertices); }

    const CollisionShape<T> &GetShape(T id) const { return m_Grid->GetShape(id); }

    void DestroyShape(T id) { m_Grid->DestroyShape(id); }

    std::vector<CollisionResult<T>> Detect(const std::vector<glm::vec3> &vertices) const {
        auto neighbors = m_Grid->FindNeighbors(vertices);

        if (neighbors.size() == 0) {
            return {};
        }

        return m_CollisionDetection.Detect(vertices, neighbors);
    }

    std::vector<CollisionResult<T>> Raycast(glm::vec3 origin, glm::vec3 direction, float distance) {
        glm::vec3 v1 = origin;
        glm::vec3 v2 = origin + direction * distance;

        auto neighbors = m_Grid->FindNeighbors({v1, v2});
        if (neighbors.size() == 0) {
            return {};
        }

        return m_CollisionDetection.Raycast(origin, direction, distance, neighbors);
    }

    std::optional<glm::vec3> RaycastTerrain(T id, glm::vec3 origin, glm::vec3 direction, float distance) const {
        glm::vec3 from = origin;
        glm::vec3 to = origin + direction * distance;

        if (from.y < to.y) {
            std::swap(from, to);
        }

        const auto &terrain = m_Grid->GetShape(id);

        do {
            glm::vec3 center = (from + to) / 2.0f;

            glm::vec3 cellSize = (terrain.box.max - terrain.box.min) / glm::vec3(terrain.columns, 1.0f, terrain.rows);
            cellSize.y = 1.0f;
            glm::vec3 positionInTerrain = (center - terrain.box.min) / cellSize;

            float terrainWidth = static_cast<float>(terrain.columns);
            float terrainHeight = static_cast<float>(terrain.rows);

            positionInTerrain.x = glm::clamp(positionInTerrain.x, 0.0f, terrainWidth);
            positionInTerrain.z = glm::clamp(positionInTerrain.z, 0.0f, terrainHeight);

            unsigned int x = static_cast<unsigned int>(std::floor(positionInTerrain.x));
            unsigned int z = static_cast<unsigned int>(std::floor(positionInTerrain.z));

            float positionInTileX = positionInTerrain.x - static_cast<float>(x);
            float positionInTileZ = positionInTerrain.z - static_cast<float>(z);

            float terrainY = 0.0f;

            if (positionInTileX <= (1 - positionInTileZ)) {
                terrainY = m_CollisionDetection.BarryCentric(
                    glm::vec3(0.0f, terrain.vertices[(z + 1) * (terrain.columns + 1) + x].y, 1.0),
                    glm::vec3(1.0f, terrain.vertices[z * (terrain.columns + 1) + x + 1].y, 0.0),
                    glm::vec3(0.0f, terrain.vertices[z * (terrain.columns + 1) + x].y, 0.0),
                    glm::vec2(positionInTileX, positionInTileZ));
            } else {
                terrainY = m_CollisionDetection.BarryCentric(
                    glm::vec3(0.0f, terrain.vertices[(z + 1) * (terrain.columns + 1) + x].y, 1.0),
                    glm::vec3(1.0f, terrain.vertices[(z + 1) * (terrain.columns + 1) + x + 1].y, 1.0),
                    glm::vec3(1.0f, terrain.vertices[z * (terrain.columns + 1) + x + 1].y, 0.0),
                    glm::vec2(positionInTileX, positionInTileZ));
            }

            if (std::abs(terrainY - center.y) < 0.01f) {
                return {glm::vec3(positionInTerrain.x, terrainY, positionInTerrain.z) * cellSize};
            }

            if (terrainY > center.y) {
                to = center;
            } else {
                from = center;
            }
        } while (from.y - to.y > 0.01f);

        return std::nullopt;
    }

  private:
    CollisionDetection<T> m_CollisionDetection;
    std::unique_ptr<Grid<T>> m_Grid;
};

} // namespace Engine