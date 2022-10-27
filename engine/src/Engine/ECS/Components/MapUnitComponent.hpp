#pragma once

#include "MapObject.hpp"
#include "Point.hpp"

#include <memory>
#include <vector>

namespace Engine {

class MapUnitComponent {
  public:
    MapObject mapObject;
    Point target;
    std::vector<Point> path;
    uint32_t pathIndex = 0;

   MapUnitComponent() : mapObject({1, 1}) {}
   MapUnitComponent(uint32_t width, uint32_t height) : mapObject({width, height}) { }
};

} // namespace Engine