#pragma once

#include "Point.hpp"

#include <optional>

namespace Engine {

struct AStarNode;

/**
 * Узел.
 */
struct AStarNode
{
    /**
     * X-координата узла.
     */
    int32_t x;

    /**
     * Y-координата узла.
     */
    int32_t y;

    /**
     * Индекс узла в open-очереди.
     */
    int32_t heapIndex;

    /**
     * Текущая итеративная стоимость узла.
     */
    uint32_t g;

    /**
     * Эвристическая оценка стоимости узла.
     */
    uint32_t h;

    /**
     * Родительский узел.
     */
    AStarNode* parent;

    AStarNode(int32_t x, int32_t y) : x(x), y(y), heapIndex(-1), g(0), h(0), parent(nullptr) {}

    AStarNode(int32_t x, int32_t y, uint32_t h) : x(x), y(y), heapIndex(-1), g(0), h(h), parent(nullptr) {}

    /**
     * Комбинированная стоимость узла (сумма значений {@link #g} и {@link #h}).
     * @return Значение комбинированной стоимости узла.
     */
    uint32_t totalCost() const {
        return g + h;
    }

    void reset() {
        parent = nullptr;
        heapIndex = -1;
        x = 0;
        y = 0;
        g = 0;
        h = 0;
    }

    void setHeapIndex(int index) {
        heapIndex = index;
    }

    int32_t getHeapIndex() const {
        return heapIndex;
    }

    bool operator>(const AStarNode& other) const {
        return totalCost() > other.totalCost();
    }
};

} // namespace Engine