#pragma once

#include "Point.hpp"

#include <vector>

namespace Engine {

class PathSmoother {
  public:
    std::vector<Point> smooth(const std::vector<Point>& path) { return path; }
};

} // namespace Engine
