#pragma once

#include "CollisionDetection.hpp"
#include "Grid.hpp"

#include <memory>

namespace Engine {

template <typename T> class Collision3D {
  public:
    Collision3D() {
        AABB gridBox(glm::vec3(-100.0f, -100.0f, -100.0f),
                     glm::vec3(100.0f, 100.0f, 100.0f));
        m_Grid = std::make_unique<Grid<T>>(gridBox);
    }

    void AddShape(T id, const std::vector<glm::vec3> &vertices) {
        m_Grid->AddShape(id, vertices);
    }

    void UpdateShape(T id, const std::vector<glm::vec3> &vertices) {
        m_Grid->UpdateShape(id, vertices);
    }

    void DestroyShape(T id) { m_Grid->DestroyShape(id); }

    std::vector<CollisionResult<T>>
    Detect(const std::vector<glm::vec3> &vertices) const {
        auto neighbors = m_Grid->FindNeighbors(vertices);

        if (neighbors.size() == 0) {
            return {};
        }

        return m_CollisionDetection.Detect(vertices, neighbors);
    }

  private:
    CollisionDetection<T> m_CollisionDetection;
    std::unique_ptr<Grid<T>> m_Grid;
};

} // namespace Engine