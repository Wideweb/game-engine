#pragma once

#include "Point.hpp"

#include <cmath>
#include <algorithm>

namespace Engine {

/**
 * Функция для эвристической оценки стоимости пути из одной точки в другую.
 */
class Heuristic {
  public:

    static Heuristic MANHATTAN;
    static Heuristic DIAGONAL;
  
    enum class Type {
        /**
         * Функция для оценки стоимости пути, когда разрешено движение только
         * параллельно оси координат.
         */
        MANHATTAN = 0,

        /**
         * Функция для оценки стоимости пути, когда разрешено движение
         * как параллельно оси координат, так и по диагонали.
         */
        DIAGONAL = 1
    };

    /**
     * Стоимость движения параллельно оси координат.
     */
    static const uint32_t D;

    /**
     * Стоимость движения по диагонали.
     */
    static const uint32_t D2;

    Type type;

    Heuristic(Type type);

    uint32_t calculate(const Point p1, const Point p2) const;

    uint32_t calculate(int x1, int y1, int x2, int y2) const;

    uint32_t calculate(const Point p1, int z1, const Point p2, int z2) const;

    uint32_t calculate(int x1, int y1, int z1, int x2, int y2, int z2) const;

  private:

    uint32_t calculateManhattan(int x1, int y1, int x2, int y2) const;

    uint32_t calculateManhattan(int x1, int y1, int z1, int x2, int y2, int z2) const;

    uint32_t calculateDiagonal(int x1, int y1, int x2, int y2) const;
        
    uint32_t calculateDiagonal(int x1, int y1, int z1, int x2, int y2, int z2) const;
};


} // namespace Engine