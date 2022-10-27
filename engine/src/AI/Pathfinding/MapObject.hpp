#pragma once

#include "Point.hpp"

namespace Engine {

struct MapObject {
    uint32_t width;
    uint32_t height;
    Point position;

    MapObject(uint32_t width, uint32_t height) : width(width), height(height), position({0, 0}) {}
};

} // namespace Engine
