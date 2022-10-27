#pragma once

#include "Edge.hpp"
#include "Node.hpp"
#include "Boundaries.hpp"
#include "Heuristic.hpp"
#include "BalancedBinaryHeap.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <unordered_set>
#include <optional>

namespace Engine {

/**
 * Используется для поиска пути по абстрактным узлам графа.
 */
class PathfinderAbstract {
  public:
    /**
     * Находит оптимальный путь из одного узла графа в другой в пределах области.
     * @param start Начальный узел.
     * @param dest Конечный узел.
     * @param heuristic Функция эвристической оценки расстояния.
     * @param boundaries Область, ограничивающая пространство поиска.
     * @return Список с ребрами графа, если путь найден, иначе - пустой список.
     */
    static std::vector<std::shared_ptr<Edge>> findPath(const std::shared_ptr<Node>& start,
                                                       const std::shared_ptr<Node>& dest,
                                                       const Heuristic heuristic,
                                                       const std::optional<Boundaries> boundaries)
    {
        return findPath(start, dest, heuristic, boundaries, false, 100);
    }

    /**
     * Находит оптимальный путь из одного узла графа в другой в пределах области.
     * @param start Начальный узел.
     * @param dest Конечный узел.
     * @param heuristic Функция эвристической оценки расстояния.
     * @param boundaries Область, ограничивающая пространство поиска.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшему к целевому узлу.
     * @param approximateNodesNumber Количество узлов в рассматриваемой области.
     * @return Список с ребрами графа, если путь найден, иначе - пустой список.
     */
    static std::vector<std::shared_ptr<Edge>> findPath(const std::shared_ptr<Node>& start,
                                                       const std::shared_ptr<Node>& dest,
                                                       const Heuristic heuristic,
                                                       const std::optional<Boundaries> boundaries,
                                                       bool findNearestToTarget,
                                                       uint32_t approximateNodesNumber) {
        std::shared_ptr<Node> nearest = start;
        uint32_t nearestDistance = std::numeric_limits<uint32_t>::max();

        std::unordered_set<std::shared_ptr<Node>> touchedSet;
        touchedSet.reserve(approximateNodesNumber);

        BalancedBinaryHeap<std::shared_ptr<Node>> openSet = BalancedBinaryHeap<std::shared_ptr<Node>>(approximateNodesNumber);

        start->pathfindingState.reset();
        start->pathfindingState.cost = 0;

        touchedSet.insert(start);
        openSet.add(start);
        while (!openSet.empty()) {
            const std::shared_ptr<Node> current = openSet.poll();
            if (current->position == dest->position && current->level == dest->level) {
                return rebuildPath(start, current);
            }

            current->pathfindingState.inClosedSet = true;

            for (uint32_t i = 0; i < current->edges.size(); i++)
            {
                const auto& edge = current->edges[i];
                const auto& neighbour = edge->end.lock();

                if (boundaries.has_value() && !boundaries.value().contains(neighbour->position)) {
                    continue;
                }

                if (touchedSet.find(neighbour) == touchedSet.end()) {
                    neighbour->pathfindingState.reset();
                    touchedSet.insert(neighbour);
                }

                if (neighbour->pathfindingState.inClosedSet) {
                    continue;
                }

                const uint32_t gCostNew = current->pathfindingState.cost + edge->weight;
                const uint32_t gCostPrev = neighbour->pathfindingState.cost;

                if (gCostNew >= gCostPrev) {
                    continue;
                }

                uint32_t heuristicDistance = heuristic.calculate(neighbour->position.x, neighbour->position.y, neighbour->level,
                        dest->position.x, dest->position.y, dest->level);

                for (const auto& neighbourEdge: neighbour->edges) {
                    // все портальные связи находятся вверху списка.
                    if (neighbourEdge->type != Edge::Type::PORTAL) {
                        break;
                    }

                    const uint32_t portalCost = heuristic.calculate(neighbourEdge->end.lock()->position, dest->position);
                    if (portalCost < heuristicDistance) {
                        heuristicDistance = portalCost;
                    }
                }

                if (findNearestToTarget && nearestDistance > heuristicDistance) {
                    nearest = neighbour;
                    nearestDistance = heuristicDistance;
                }

                neighbour->pathfindingState.transition = edge;
                neighbour->pathfindingState.cost = gCostNew;
                neighbour->pathfindingState.totalCost = gCostNew + heuristicDistance;

                if (openSet.contains(neighbour)) {
                    openSet.update(neighbour);
                } else {
                    openSet.add(neighbour);
                }
            }
        }

        if (findNearestToTarget && nearest != nullptr) {
            return rebuildPath(start, nearest);
        }

        return {};
    }

  private:

    /**
     * Составляет путь из начального узла в конечный в виде списка ребер графа.
     * @param start Начальный узел.
     * @param dest Конечный узел.
     * @return Путь из начального узла в конечный в виде списка ребер графа.
     */
    static std::vector<std::shared_ptr<Edge>> rebuildPath(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& dest) {
        std::vector<std::shared_ptr<Edge>> path;

        std::shared_ptr<Node> current = dest;
        while (current != start) {
            path.push_back(current->pathfindingState.transition);
            current = current->pathfindingState.transition->start.lock();
        }

        std::reverse(path.begin(), path.end());
        return path;
    }
};

} // namespace Engine