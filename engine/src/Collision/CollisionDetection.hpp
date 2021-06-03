#pragma once

#include "BroadPhaseAlgorithm.hpp"
#include "CollisionShape.hpp"
#include "Math.hpp"
#include "NarrowPhaseAlgorithm.hpp"

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include <cmath>

namespace Engine {

template <typename T> struct CollisionResult {
    T id;
    glm::vec3 mtv;

    CollisionResult() {}
    CollisionResult(T id) : id(id) {}
    CollisionResult(T id, glm::vec3 mtv) : id(id), mtv(std::move(mtv)) {}
};

template <typename T> class CollisionDetection {
  private:
    BroadPhaseAlgorithm<T> m_BroadPhase;
    NarrowPhaseAlgorithm m_NarrowPhase;

  public:
    std::vector<CollisionResult<T>> Detect(const std::vector<glm::vec3> &vertices,
                                           const std::vector<CollisionShape<T>> &shapes) const {

        std::vector<CollisionShape<T>> overlaps = m_BroadPhase.ComputeOverlaps(vertices, shapes);

        std::vector<CollisionResult<T>> result;
        result.reserve(overlaps.size());

        glm::vec3 offset = glm::vec3(0.0f);

        AABB aabb{vertices};
        glm::vec3 center = aabb.min + (aabb.max - aabb.min) / 2.0f;
        center.y = aabb.min.y;

        for (const auto &overlapedShape : overlaps) {
            if (overlapedShape.type == CollisionShapeType::Terrain) {
                glm::vec3 positionInTerrain = center - overlapedShape.box.min;
                float terrainWidth = static_cast<float>(overlapedShape.width);
                float terrainHeight = static_cast<float>(overlapedShape.height);

                positionInTerrain.x = glm::clamp(positionInTerrain.x, 0.0f, terrainWidth);
                positionInTerrain.z = glm::clamp(positionInTerrain.z, 0.0f, terrainHeight);

                unsigned int x = static_cast<unsigned int>(std::floor(positionInTerrain.x));
                unsigned int z = static_cast<unsigned int>(std::floor(positionInTerrain.z));

                float positionInTileX = positionInTerrain.x - static_cast<float>(x);
                float positionInTileZ = positionInTerrain.z - static_cast<float>(z);

                auto &vertices = overlapedShape.vertices;
                float terrainY = 0.0f;

                if (positionInTileX <= (1 - positionInTileZ)) {
                    terrainY = BarryCentric(glm::vec3(0.0f, vertices[(z + 1) * overlapedShape.width + x].y, 1.0),
                                            glm::vec3(1.0f, vertices[z * overlapedShape.width + x + 1].y, 0.0),
                                            glm::vec3(0.0f, vertices[z * overlapedShape.width + x].y, 0.0),
                                            glm::vec2(positionInTileX, positionInTileZ));
                } else {
                    terrainY = BarryCentric(glm::vec3(0.0f, vertices[(z + 1) * overlapedShape.width + x].y, 1.0),
                                            glm::vec3(1.0f, vertices[(z + 1) * overlapedShape.width + x + 1].y, 1.0),
                                            glm::vec3(1.0f, vertices[z * overlapedShape.width + x + 1].y, 0.0),
                                            glm::vec2(positionInTileX, positionInTileZ));
                }

                if (terrainY > center.y) {
                    glm::vec3 mtv = glm::vec3(0.0f, terrainY - center.y, 0.0f);

                    result.emplace_back(overlapedShape.id, mtv);
                    offset += mtv;
                }

                continue;
            }

            glm::vec3 mtv = m_NarrowPhase.Collide(vertices, overlapedShape.vertices, offset);

            if (!Math::isEqual(mtv.x + mtv.y + mtv.z, 0.0f)) {
                result.emplace_back(overlapedShape.id, mtv);
                offset += mtv;
            }
        }

        return result;
    }

    float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) const {
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
        float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
    }

    std::vector<CollisionResult<T>> Raycast(glm::vec3 origin, glm::vec3 direction, float distance,
                                            const std::vector<CollisionShape<T>> &shapes) const {
        glm::vec3 p0 = origin;
        glm::vec3 p1 = origin + direction * distance;

        std::vector<CollisionShape<T>> overlaps = m_BroadPhase.ComputeOverlaps({p0, p1}, shapes);

        std::vector<CollisionResult<T>> result;
        result.reserve(overlaps.size());

        for (const auto &overlapedShape : overlaps) {
            if (overlapedShape.type == CollisionShapeType::Terrain) {
                continue;
            }

            if (AABBOverlap::testSegment(origin, origin + direction * distance, overlapedShape.box)) {
                result.emplace_back(overlapedShape.id);
            }
        }

        return result;
    }
};

} // namespace Engine