#pragma once

#include "Point.hpp"

#include <vector>
#include <memory>

namespace Engine {

struct Node;

/**
 * Ребро связывающее два узла графа.
 */
struct Edge {
    /**
     * Тип ребра.
     */
    enum class Type {
        /**
         * Соединяет пару узлов внутри одной области.
         */
        INTRA = 0,

        /**
         * Соединяет пару узлов, которые принадлежат разным областям.
         */
        INTER = 1,

        /**
         * Соединяет пару узлов.
         */
        PORTAL = 2
    };

    /**
     * Начало ребра.
     */
    std::weak_ptr<Node> start;

    /**
     * Конец ребра.
     */
    std::weak_ptr<Node> end;

    /**
     * Тип ребра.
     */
    Type type;

    /**
     * Стоимость пересечения ребра.
     */
    uint32_t weight;

    /**
     * Путь по абстрактным узлам графа.
     */
    std::vector<std::shared_ptr<Edge>> underlyingPath;

    /**
     * Путь по ячейкам карты.
     */
    std::vector<Point> underlyingMapPath;

    Edge (std::weak_ptr<Node> start, std::weak_ptr<Node> end, Type type, uint32_t weight)
        : start(start), end(end), type(type), weight(weight) {}
};

} // namespace Engine