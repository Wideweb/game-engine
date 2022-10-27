#pragma once

#include "Point.hpp"

namespace Engine {

struct Direction : Point {
    using Point::Point;

    static Direction UP;
    static Direction DOWN;
    static Direction LEFT;
    static Direction RIGHT;
    static Direction LEFT_UP;
    static Direction LEFT_DOWN;
    static Direction RIGHT_UP;
    static Direction RIGHT_DOWN;

    bool alongAxes() const;
    static const Direction detect(Point start, Point end);
};

} // namespace Engine
