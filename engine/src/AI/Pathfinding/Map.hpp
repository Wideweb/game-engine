#pragma once

#include "Point.hpp"
#include "MapObject.hpp"
#include "Direction.hpp"
#include "Boundaries.hpp"

#include <array>
#include <vector>

namespace Engine {

class Map {
  private:
    struct Cell {
        Point position;
        bool locked;

        Cell(): position({0, 0}), locked(false) {}
        Cell(Point position, bool locked): position(position), locked(locked) {}
    };

    /**
     * Направления, в которых разрешено перемещение.
     */
    const std::array<Direction, 8> c_Directions = {
        Direction::LEFT_UP,
        Direction::UP,
        Direction::RIGHT_UP,
        Direction::LEFT,
        Direction::RIGHT,
        Direction::LEFT_DOWN,
        Direction::DOWN,
        Direction::RIGHT_DOWN
    };

  public:
    
    uint32_t width;
    uint32_t height;

    std::vector<Cell> cells;

    Map(uint32_t width, uint32_t height);

    bool out(Point point) const;

    bool canPlace(const MapObject& object, Point position) const;

    bool canMove(const MapObject& object, Point position, Direction direction) const;

    bool allowed(Direction direction) const;

    bool isFree(Point position) const;

    bool isLocked(Point position) const;

    void lock(Point position);

    void lock(Boundaries boundaries);

    const std::array<Direction, 8>& directions() const;

  private:
    const Cell& getCell(Point position) const;
    Cell& getCell(Point position);
};

} // namespace Engine
