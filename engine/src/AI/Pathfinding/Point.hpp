#pragma once

#include <functional>

namespace Engine {

struct Point {
    int32_t x;
    int32_t y;

    Point() : x(0), y(0) {}
    Point(int32_t x, int32_t y) : x(x), y(y) {}

    bool operator==(Point other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(Point other) const {
        return !(*this == other);
    }

    Point operator+(Point other) const {
        return {x + other.x, y + other.y};
    }
};

struct PointHasher {
    size_t operator()(Point point) const {
        return point.y * 500 + point.x;
    }
};

} // namespace Engine
