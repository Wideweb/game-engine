#pragma once

#include "Point.hpp"

#include <vector>

namespace Engine {

class PathFinderResultHandler {
  public:
    /**
     * Вызывается, когда найден путь по ячейкам игровой карты между начальной и конечной точкой.
     * @param path путь по ячейкам игровой карты между начальной и конечной точкой.
     */
    virtual void onPathFound(const std::vector<Point>& path) = 0;

    /**
     * Вызывается, когда путь не был найден.
     */
    virtual void onPathNotFound() = 0;
};

} // namespace Engine
