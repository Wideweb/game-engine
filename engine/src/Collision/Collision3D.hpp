#pragma once

#include "CollisionDetection.hpp"
#include "CollisionShape.hpp"
#include "Grid.hpp"

#include <memory>

namespace Engine {

template <typename T> class Collision3D {
  public:
    Collision3D() {
        AABB gridBox(glm::vec3(-100.0f, -100.0f, -100.0f), glm::vec3(100.0f, 100.0f, 100.0f));
        m_Grid = std::make_unique<Grid<T>>(gridBox);
    }

    void AddTerrain(T id, const std::vector<glm::vec3> &vertices, unsigned int width, unsigned int height) {
        CollisionShape<T> shape(id, vertices, CollisionShapeType::Terrain);
        shape.width = width;
        shape.height = height;
        m_Grid->AddShape(shape);
    }

    void AddShape(T id, const std::vector<glm::vec3> &vertices) {
        CollisionShape<T> shape(id, vertices, CollisionShapeType::Object);
        m_Grid->AddShape(shape);
    }

    void UpdateShape(T id, const std::vector<glm::vec3> &vertices) { m_Grid->UpdateShape(id, vertices); }

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

  private:
    CollisionDetection<T> m_CollisionDetection;
    std::unique_ptr<Grid<T>> m_Grid;
};

} // namespace Engine