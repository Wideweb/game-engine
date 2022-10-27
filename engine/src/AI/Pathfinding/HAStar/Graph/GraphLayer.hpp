#pragma once

#include "Cluster.hpp"

#include <vector>

namespace Engine {

/**
 * Уровень иерархии графа, который состоит из областей.
 */
struct GraphLayer {
    /**
     * Номер уровня.
     */
    uint32_t level;

    /**
     * Количество областей по горизонтали.
     */
    uint32_t width;

    /**
     * Количество областей по вертикали.
     */
    uint32_t height;

    /**
     * Список областей текущего уровня.
     */
    std::vector<std::shared_ptr<Cluster>> clusters;

    /**
     * Создает уровень иерархии.
     * @param level Уровень иерархии.
     * @param height Количество областей по вертикали.
     * @param width Количество областей по горизонтали.
     * @param clusters Список областей текущего уровня.
     */
    GraphLayer(uint32_t level, uint32_t height, uint32_t width, std::vector<std::shared_ptr<Cluster>>& clusters) 
        : level(level), height(height), width(width), clusters(std::move(clusters))
    { }
};

} // namespace Engine