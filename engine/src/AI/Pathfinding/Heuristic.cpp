#include "Heuristic.hpp"

namespace Engine {

Heuristic Heuristic::MANHATTAN = Heuristic(Heuristic::Type::MANHATTAN);
Heuristic Heuristic::DIAGONAL = Heuristic(Heuristic::Type::DIAGONAL);

const uint32_t Heuristic::D = 10;
const uint32_t Heuristic::D2 = 14;

Heuristic::Heuristic(Type type) : type(type) { }

uint32_t Heuristic::calculate(const Point p1, const Point p2) const {
    return calculate(p1.x, p1.y, p2.x, p2.y);
}

uint32_t Heuristic::calculate(int x1, int y1, int x2, int y2) const {
    switch (type)
    {
    case Type::MANHATTAN:
        return calculateManhattan(x1, y1, x2, y2);
        break;

    case Type::DIAGONAL:
        return calculateDiagonal(x1, y1, x2, y2);
        break;
    
    default:
        return 0;
    }
}

uint32_t Heuristic::calculate(const Point p1, int z1, const Point p2, int z2) const {
    return calculate(p1, z1, p2, z2);
}

uint32_t Heuristic::calculate(int x1, int y1, int z1, int x2, int y2, int z2) const {
    switch (type) {
    case Type::MANHATTAN:
        return calculateManhattan(x1, y1, z1, x2, y2, z2);
        break;

    case Type::DIAGONAL:
        return calculateDiagonal(x1, y1, z1, x2, y2, z2);
        break;
    
    default:
        return 0;
    }
}

uint32_t Heuristic::calculateManhattan(int x1, int y1, int x2, int y2) const {
    uint32_t dx = std::abs(x1 - x2);
    uint32_t dy = std::abs(y1 - y2);
    return D * (dx + dy);
}

uint32_t Heuristic::calculateManhattan(int x1, int y1, int z1, int x2, int y2, int z2) const {
    uint32_t dx = std::abs(x1 - x2);
    uint32_t dy = std::abs(y1 - y2);
    uint32_t dz = std::abs(z1 - z2);
    return D * (dx + dy + dz);
}

uint32_t Heuristic::calculateDiagonal(int x1, int y1, int x2, int y2) const {
    uint32_t dx = std::abs(x1 - x2);
    uint32_t dy = std::abs(y1 - y2);
    return D * (dx + dy) + (D2 - D * 2) * std::min(dx, dy);
}

    
uint32_t Heuristic::calculateDiagonal(int x1, int y1, int z1, int x2, int y2, int z2) const {
    uint32_t dx = std::abs(x1 - x2);
    uint32_t dy = std::abs(y1 - y2);
    uint32_t dz = std::abs(z1 - z2);
    return D * (dx + dy + dz) + (D2 - D * 2) * (std::min(dx, dy) + std::max(std::min(dy, dz), std::min(dz, dx)));
}

} // namespace Engine