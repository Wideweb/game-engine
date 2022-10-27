#pragma once

#include "Point.hpp"

namespace Engine {

struct Boundaries {
    Point min;
    Point max;

    Boundaries() : min({0, 0}), max({0, 0}) {}

    Boundaries(Point min, Point max) : min(min), max(max) {}

    /**
     * Проверяет, содержит ли эта область другую.
     * @param other Область.
     * @return true - если область содержит переданную, иначе - false.
     */
    bool contains(Boundaries other) const {
        return min.x <= other.min.x && min.y <= other.min.y && other.max.x <= max.x && other.max.y <= max.y;
    }

    /**
     * Проверяет, содержит ли эта область ячейку.
     * @param point ячейка.
     * @return true - если область содержит ячейку, иначе - false.
     */
    bool contains(Point point) const {
        return min.x <= point.x && point.x <= max.x && min.y <= point.y && point.y <= max.y;
    }

    /**
     * Проверяет, содержит ли эта область ячейку.
     * @param x координата ячейки.
     * @param y координата ячейки.
     * @return true - если область содержит ячейку, иначе - false.
     */
    bool contains(int32_t x, int32_t y) const {
        return min.x <= x && x <= max.x && min.y <= y && y <= max.y;
    }

    /**
     * Количество ячеек карты по горизонтали.
     */
    int32_t width() const {
        return max.x - min.x + 1;
    }

    /**
     * Количество ячеек карты по вертикали.
     */
    int32_t height() const {
        return max.y - min.y + 1;
    }

    bool operator==(const Boundaries& other) const {
        return min == other.min && max == other.max;
    }
};

} // namespace Engine
