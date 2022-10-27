#include "Map.hpp"

namespace Engine {

Map::Map(uint32_t width, uint32_t height) : width(width), height(height) {
    cells.reserve(height * width);
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            Point position = Point(x, y);
            cells.emplace_back(position, false);
        }
    }
}

bool Map::out(Point point) const {
    return point.x < 0 || point.y < 0 || point.x >= width || point.y >= height;
}

bool Map::canPlace(const MapObject& object, Point position) const {
    for (uint32_t dy = 0; dy < object.height; dy++) {
        for (uint32_t dx = 0; dx < object.width; dx++) {
            if (isLocked(Point(static_cast<int32_t>(dx), static_cast<int32_t>(dy)) + position)) {
                return false;
            }
        }
    }
    return true;
}

bool Map::canMove(const MapObject& object, Point position, Direction direction) const {
    return canPlace(object, position + direction);
}

bool Map::allowed(Direction direction) const {
    return true;
}

bool Map::isFree(Point position) const {
    return !getCell(position).locked;
}

bool Map::isLocked(Point position) const {
    return getCell(position).locked;
}

void Map::lock(Point position) {
    getCell(position).locked = true;
}

void Map::lock(Boundaries boundaries) {
    for (int32_t y = boundaries.min.y; y <= boundaries.max.y; y++) {
        for (int32_t x = boundaries.min.x; x <= boundaries.max.x; x++) {
            lock({x, y});
        }
    }
}

const Map::Cell& Map::getCell(Point position) const {
    return cells.at(position.y * width + position.x);
}

Map::Cell& Map::getCell(Point position) {
    return cells[position.y * width + position.x];
}

const std::array<Direction, 8>& Map::directions() const {
    return c_Directions;
}

} // namespace Engine
