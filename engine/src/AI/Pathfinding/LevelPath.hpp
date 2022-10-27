#pragma once

#include "Point.hpp"

#include <vector>

namespace Engine {

/**
 * Хранит путь, который был построен на определенном уровне карты. Является частью многоуровнего пути.
 */
struct LevelPath {
    /**
     * Тип перемещения.
     */
    enum class Type {
        /**
         * Путь внутри одного уровня.
         */
        INTRA,

        /**
         * Межуровневое перемещение.
         */
        INTER,
    };

    /**
     * Уровень.
     */
    int32_t level;

    /**
     * Путь.
     */
    std::vector<Point> path;

    /**
     * Тип пути.
     */
    Type type;

    /**
     * Путь по уровню карты.
     * @param level Уровень карты.
     */
    LevelPath(int32_t level) : level(level), type(Type::INTRA) { }

    /**
     * Путь по уровню карты.
     * @param level Уровень карты.
     * @param point Путь в виде одной клетки.
     * @param type Тип пути.
     */
    LevelPath(int32_t level, Point point, Type type) : level(level), type(type) {
        this->path.push_back(point);
    }

    /**
     * Путь по уровню карты.
     * @param level Уровень карты.
     * @param path Путь.
     * @param type Тип пути.
     */
    LevelPath(int32_t level, std::vector<Point> path, Type type) : level(level), path(path), type(type) { }
};

} // namespace Engine