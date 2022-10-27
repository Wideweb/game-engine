#pragma once

#include "Node.hpp"
#include "Boundaries.hpp"
#include "FlatDictionary.hpp"
#include "Point.hpp"

#include <vector>
#include <memory>

namespace Engine {

/**
 * Абстрактная область карты. Может объединять как ячейки карты, так и другие области на уровне ниже по иерархии.
 */
struct Cluster
{
    /**
     * Границы область (относительно исходной карты).
     */
    Boundaries boundaries;

    /**
     * Пограничные узлы области.
     */
    FlatDictionary<Point, std::shared_ptr<Node>, PointHasher> borders;

    /**
     * Порталы.
     */
    FlatDictionary<Point, std::shared_ptr<Node>, PointHasher> portals;

    /**
     * Области на уровне ниже по иерархии, которые входят в текущую область.
     */
    std::vector<std::shared_ptr<Cluster>> clusters;

    /**
     * Проверяет, содержит ли эта область другую.
     * @param other Область.
     * @return true - если область содержит переданную, иначе - false.
     */
    bool contains(const std::shared_ptr<Cluster>& other) {
        return boundaries.contains(other->boundaries);
    }

    /**
     * Проверяет, содержит ли эта область ячейку.
     * @param pos ячейка.
     * @return true - если область содержит ячейку, иначе - false.
     */
    bool contains(Point pos) const {
        return boundaries.contains(pos);
    }

    bool operator==(const Cluster& other) const {
        return this->boundaries == other.boundaries;
    }

    bool operator!=(const Cluster& other) const {
        return !(*this == other);
    }
};

} // namespace Engine