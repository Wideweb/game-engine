#include "Direction.hpp"

namespace Engine {

Direction Direction::UP = Direction(0, 1);
Direction Direction::DOWN = Direction(0, -1);
Direction Direction::LEFT = Direction(-1, 0);
Direction Direction::RIGHT = Direction(1, 0);
Direction Direction::LEFT_UP = Direction(-1, 1);
Direction Direction::LEFT_DOWN = Direction(-1, -1);
Direction Direction::RIGHT_UP = Direction(1, 1);
Direction Direction::RIGHT_DOWN = Direction(1, -1);

bool Direction::alongAxes() const {
    return x * y == 0;
}

const Direction Direction::detect(Point start, Point end) {
    int32_t dx = end.x - start.x;
    int32_t dy = end.y - start.y;

    if (dx == 0 && dy < 0)
    {
        return UP;
    }
    else if (dx < 0 && dy == 0)
    {
        return LEFT;
    }
    else if (dx > 0 && dy == 0)
    {
        return RIGHT;
    }
    else if (dx == 0 && dy > 0)
    {
        return DOWN;
    }
    else if (dx < 0 && dy < 0)
    {
        return LEFT_UP;
    }
    else if (dx < 0 && dy > 0)
    {
        return LEFT_DOWN;
    }
    else if (dx > 0 && dy < 0)
    {
        return RIGHT_UP;
    }
    else
    {
        return RIGHT_DOWN;
    }
}

} // namespace Engine
