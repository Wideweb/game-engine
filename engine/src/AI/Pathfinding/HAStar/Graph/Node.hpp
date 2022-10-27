#pragma once

#include "Point.hpp"
#include "Edge.hpp"

#include <limits>
#include <deque>
#include <memory>
#include <functional>

namespace Engine {

struct Node {
    enum class State {
        /**
         * Узел новый.
         */
        NEW,

        /**
         * Узел связан с графом.
         */
        LINKED,

        /**
         * Узел нужно проверить.
         */
        CHECK,

        /**
         * Узел нужно заменить.
         */
        REPLACE,

        /**
         * Узел удален.
         */
        REMOVED
    };

    /**
     * Состояние узла, которое используется во время работы алгоритма поиска пути А*.
     */
    struct AStarState {
        /**
         * Узел просмотрен.
         */
        bool inClosedSet = false;

        /**
         * Стоимость пути от начального узла.
         */
        uint32_t cost = std::numeric_limits<uint32_t>::max();

        /**
         * Приоритет узла.
         */
        uint32_t totalCost = 0;

        /**
         * Связь из предыдущего узла.
         */
        std::shared_ptr<Edge> transition;

        /**
         * Индекс узла в очереди на рассмотрение.
         */
        int32_t heapIndex = -1;

        /**
         * Сбрасывает состояние узла.
         */
        void reset() {
            inClosedSet = false;
            cost = std::numeric_limits<uint32_t>::max();
            totalCost = 0;
            transition = nullptr;
            heapIndex = -1;
        }
    };

    /**
     * Ячейка карты, которую представляет узел.
     */
    Point position;

    /**
     * Координата ячейки узла в третьем пространстве.
     */
    uint32_t level;

    /**
     * Связи с другими узлами графа.
     */
    std::deque<std::shared_ptr<Edge>> edges;

    /**
     * Представление узла на уровне иерархии графа ниже.
     */
    std::shared_ptr<Node> child;

    /**
     * Состояние узла, которое используется во время работы алгоритма поиска пути А*.
     */
    AStarState pathfindingState;

    /**
     * Состояние узла.
     */
    State state = State::NEW;

    /**
     * Создает узел для указанной ячейки карты.
     */
    Node() { }

    /**
     * Создает узел для указанной ячейки карты.
     * @param position Ячейка карты.
     */
    Node(Point position) : position(position), level(0){ }

    /**
     * Создает узел для указанной ячейки карты.
     * @param position Ячейка карты.
     * @param level Z ячейки карты.
     */
    Node(Point position, uint32_t level) : position(position), level(level){ }

    /**
     * Находит связь с узлом.
     * @param to Узел.
     * @return Ребро, если связь с узлом была найдена, иначе - null.
     */
    std::shared_ptr<Edge> findEdge(const std::shared_ptr<Node>& to) const {
        for (auto& edge: edges) {
            if (edge->end.lock()->position == to->position) {
                return edge;
            }
        }

        return nullptr;
    }

    /**
     * Удаляет связь определенного типа с указанным узлом.
     * @param to Узел.
     * @param type Тип связи.
     */
    void removeEdge(Point to, Edge::Type type) {
        removeEdge(to, type, level);
    }

    /**
     * Удаляет связь определенного типа с указанным узлом.
     * @param to Узел.
     * @param type Тип связи.
     * @param level Уровень, на котором находится узел.
     */
    void removeEdge(Point to, Edge::Type type, uint32_t level) {
        for (int i = 0; i < edges.size(); i++) {
            auto& edge = edges[i];
            auto end = edge->end.lock();
            if (edge->type == type && end->position == to && end->level == level) {
                edges[i] = edges[edges.size() - 1];
                edges.pop_back();
                return;
            }
        }
    }

    /**
     * Проверяет наличие связи у узла.
     * @param type Тип связи.
     */
    bool hasEdge(Edge::Type type) const {
        for (const auto& edge: edges) {
            if (edge->type == type) {
                return true;
            }
        }

        return false;
    }

    void setHeapIndex(int index) {
        pathfindingState.heapIndex = index;
    }

    int32_t getHeapIndex() const {
        return pathfindingState.heapIndex;
    }

    bool operator>(const Node& o) const {
        return pathfindingState.totalCost > o.pathfindingState.totalCost;
    }
};

} // namespace Engine