#pragma once

#include "PathfindingGraph.hpp"
#include "LevelPath.hpp"
#include "Point.hpp"
#include "PathSmoother.hpp"
#include "PathfinderAbstract.hpp"
#include "HPathFinderResultHandler.hpp"
#include "HLevelPathFinderResultHandler.hpp"

#include <vector>
#include <deque>
#include <utility>
#include <iterator>
#include <memory>
#include <cmath>
#include <limits>

namespace Engine {

class HierarchicalPathfinder {
  public:
    /**
     * Находит путь между начальной и конечной точкой в графе поиска пути.
     * @param levels Уровни.
     * @param start Начальная точка пути.
     * @param dest Конечная точка пути.
     * @param smoother Алгоритм сглаживания пути.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшей к целевой точке.
     * @param handler Обработчик результатов поиска.
     */
    static void findMultiLevelPath(std::vector<PathfindingGraph>& levels,
                                   uint32_t startLevel,
                                   const Point start,
                                   uint32_t destLevel,
                                   const Point dest,
                                   const std::shared_ptr<PathSmoother>& smoother,
                                   bool findNearestToTarget,
                                   const std::shared_ptr<HLevelPathFinderResultHandler>& handler) {
        if (levels.size() <= startLevel || levels.size() <= destLevel) {
            handler->onLevelPathNotFound();
            return;
        }

        PathfindingGraph& startGraph = levels[startLevel];
        PathfindingGraph& destGraph = levels[destLevel];

        if (!findNearestToTarget && !destGraph.map.canPlace(destGraph.object, dest)) {
            handler->onLevelPathNotFound();
            return;
        }

        // Добавляем узлы в графы поиска для стартовой и целевой точек.
        std::shared_ptr<Node> startNode;
        std::shared_ptr<Node> destNode;

        // Если точки находятся на одном уровне, добавляем их вместе в один граф, чтобы попытаться соединить их в
        // рамках одной области и избежать лишних перемещений к граничным узлам.
        if (startLevel == destLevel) {
            const auto pair = startGraph.insertPathNodes(start, dest);
            if (pair.first == nullptr || pair.second == nullptr)
            {
                handler->onLevelPathNotFound();
                return;
            }

            startNode = pair.first;
            destNode = pair.second;
        } else {
            startNode = startGraph.insertPathNode(start);
            destNode = destGraph.insertPathNode(dest);
        }

        const std::vector<std::shared_ptr<Node>> abstractPath = findAbstractMultiLevelPath(startGraph, startNode, destNode, findNearestToTarget);
        if (abstractPath.empty()) {
            handler->onLevelPathNotFound();
        }
        // Останавливаем поиск, если клиент решил не детализировать путь.
        else if (!handler->onLevelAbstractPathFound(abstractPath)) {
            // Удаляет добавленные узлы из графа.
            startGraph.removeTempNodes();
            destGraph.removeTempNodes();
            return;
        }

        const std::vector<LevelPath> concretePath = findConcreteMultiLevelPath(abstractPath, levels, startLevel, start, dest,
                smoother, findNearestToTarget);
        if (concretePath.empty()) {
            handler->onLevelPathNotFound();
        } else {
            handler->onLevelPathFound(concretePath);
        }

        // Удаляет добавленные узлы из графа.
        startGraph.removeTempNodes();
        destGraph.removeTempNodes();
    }

    /**
     * Находит путь между начальной и конечной точкой в графе поиска пути.
     * @param graph Граф поиска пути.
     * @param start Начальная точка пути.
     * @param dest Конечная точка пути.
     * @param smoother Алгоритм сглаживания пути.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшей к целевой точке.
     * @param handler обработчик результатов поиска.
     */
    static void findPath(PathfindingGraph& graph,
                         Point start,
                         Point dest, 
                         const std::shared_ptr<PathSmoother>& smoother,
                         bool findNearestToTarget,
                         const std::shared_ptr<HPathFinderResultHandler>& handler) {
        if (!findNearestToTarget && !graph.map.canPlace(graph.object, dest)) {
            handler->onPathNotFound();
            return;
        }

        const auto nodes = graph.insertPathNodes(start, dest);
        if (nodes.first == nullptr || nodes.second == nullptr) {
            handler->onPathNotFound();
            return;
        }

        const std::vector<std::shared_ptr<Node>> abstractPath = findAbstractPath(nodes.first, nodes.second, graph, findNearestToTarget);
        if (abstractPath.empty()) {
            handler->onPathNotFound();
        }
        // Останавливаем поиск, если клиент решил не детализировать путь.
        else if (!handler->onAbstractPathFound(abstractPath)) {
            graph.removeTempNodes();
            return;
        }

        const std::vector<Point> concretePath = findConcretePath(abstractPath, start, dest, graph, smoother, findNearestToTarget);
        if (concretePath.empty()) {
            handler->onPathNotFound();
        } else {
            handler->onPathFound(concretePath);
        }

        graph.removeTempNodes();
    }

  private:
    /**
     * Находит абстрактный путь по узлам графа между начальной и конечной точкой.
     * @param graph Граф поиска пути.
     * @param startNode Начальная точка пути.
     * @param destNode Конечная точка пути.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшей к целевой точке.
     */
    static std::vector<std::shared_ptr<Node>> findAbstractPath(const std::shared_ptr<Node>& startNode,
                                                               const std::shared_ptr<Node>& destNode, 
                                                               PathfindingGraph& graph,
                                                               bool findNearestToTarget) {
        // Ищем абстрактный путь.
        auto hierarchicalAbstractPath = PathfinderAbstract::findPath(startNode, destNode,
            graph.heuristic, std::nullopt, findNearestToTarget, 100);
        if (hierarchicalAbstractPath.empty())
        {
            return {};
        }

        return flatHierarchicalPath(hierarchicalAbstractPath);
    }

    /**
     * Преобразует абстрактный путь по узлам графа в путь по ячейкам на игровой карте.
     * @param abstractPath Абстрактный путь.
     * @param graph Граф поиска пути.
     * @param start Начальная точка пути.
     * @param dest Конечная точка пути.
     * @param smoother Алгоритм сглаживания пути.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшей к целевой точке.
     */
    static std::vector<Point> findConcretePath(const std::vector<std::shared_ptr<Node>>& abstractPath, 
                                               const Point start,
                                               const Point dest,
                                               PathfindingGraph& graph,
                                               const std::shared_ptr<PathSmoother>& smoother,
                                               bool findNearestToTarget)
    {
        if (abstractPath.empty()) {
            return {};
        }

        // Детализируем абстрактный путь.
        std::vector<Point> path = getConcretePath(abstractPath, graph, findNearestToTarget);

        if (findNearestToTarget) {
            if (path.empty()) {
                path.push_back(start);
            }

            // Если цель не достигнута, перестраиваем путь, чтобы оказаться ближе к ней.
            bool destReached = path[path.size() - 1] == dest;
            if (!destReached) {
                toNearest(path, dest, graph);
            }
        }

        // Путь не найден.
        if (!findNearestToTarget && path.empty()) {
            return {};
        }

        if (smoother != nullptr) {
            smoother->smooth(path);
        }

        return path;
    }

    /**
     * Находит абстрактный путь по узлам графа между начальной и конечной точкой.
     * @param startGraph Граф поиска пути, в котором находится начальная точка.
     * @param startNode Начальная точка пути.
     * @param destNode Конечная точка пути.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшей к целевой точке.
     */
    static std::vector<std::shared_ptr<Node>> findAbstractMultiLevelPath(PathfindingGraph& startGraph,
                                                                         const std::shared_ptr<Node>& startNode,
                                                                         const std::shared_ptr<Node> destNode,
                                                                         bool findNearestToTarget) {
        if (startNode == nullptr || destNode == nullptr) {
            return {};
        }

        // Ищем абстрактный путь.
        auto hierarchicalAbstractPath = PathfinderAbstract::findPath(startNode, destNode,
                startGraph.heuristic, std::nullopt, findNearestToTarget, 100);

        return flatHierarchicalPath(hierarchicalAbstractPath);
    }

    /**
     * Преобразует абстрактный путь по узлам графа в путь по ячейкам на игровой карте.
     * @param abstractPath Абстрактный путь.
     * @param levels Уровни.
     * @param start Начальная точка пути.
     * @param dest Конечная точка пути.
     * @param smoother Алгоритм сглаживания пути.
     * @param findNearestToTarget Если путь не будет найден, найдет путь к ближайшей к целевой точке.
     */
    static std::vector<LevelPath> findConcreteMultiLevelPath(const std::vector<std::shared_ptr<Node>>& abstractPath,
                                                             std::vector<PathfindingGraph>& levels,
                                                             int startLevel,
                                                             const Point start,
                                                             const Point dest,
                                                             const std::shared_ptr<PathSmoother>& smoother,
                                                             bool findNearestToTarget) {
        if (abstractPath.empty()) {
            return {};
        }

        std::vector<LevelPath> path;

        // Разбиваем абстрактный путь на уровни и детализируем.
        uint32_t pathFromIndex = 0;
        for (uint32_t i = 1; i < abstractPath.size(); i++) {
            const std::shared_ptr<Node>& prevNode = abstractPath[i - 1];
            const std::shared_ptr<Node>& nextNode = abstractPath[i];

            if (prevNode->level != nextNode->level) {
                std::vector<Point> levelPath = getConcretePath({abstractPath.begin() + pathFromIndex, abstractPath.begin() + i}, levels[prevNode->level], findNearestToTarget);
                path.emplace_back(prevNode->level, levelPath, LevelPath::Type::INTRA);
                // Выделяем межуровневое перемещение.

                std::vector<Point> portalPath;
                portalPath.push_back(prevNode->position);
                portalPath.push_back(nextNode->position);
                path.emplace_back(nextNode->level, std::move(portalPath), LevelPath::Type::INTER);

                pathFromIndex = i;
            }
        }

        if (pathFromIndex < abstractPath.size()) {
            const std::shared_ptr<Node>& node = abstractPath[pathFromIndex];
            const std::vector<Point> levelPath = getConcretePath({abstractPath.begin() + pathFromIndex, abstractPath.end()}, levels[node->level], findNearestToTarget);
            path.emplace_back(node->level, levelPath, LevelPath::Type::INTRA);
        }

        if (findNearestToTarget) {
            if (path.empty()) {
                path.emplace_back(startLevel, start, LevelPath::Type::INTRA);
            }
            LevelPath& levelPath = path[path.size() - 1];

            // Если цель не достигнута, перестраиваем путь, чтобы оказаться ближе к ней.
            bool destReached = levelPath.path[levelPath.path.size() - 1] == dest;
            if (!destReached) {
                toNearest(levelPath.path, dest, levels[levelPath.level]);
            }
        }

        // Путь не найден.
        if (!findNearestToTarget && path.empty()) {
            return {};
        }

        if (smoother != nullptr) {
            for (const LevelPath& levelPath: path) {
                smoother->smooth(levelPath.path);
            }
        }

        return path;
    }

    /**
     * Преобразует путь по иерархии узлов графа в линейный список узлов нижнего уровня.
     * @param hierarchicalAbstractPath Абстрактный путь.
     * @return Линейный список узлов нижнего уровня.
     */
    static std::vector<std::shared_ptr<Node>> flatHierarchicalPath(std::vector<std::shared_ptr<Edge>>& hierarchicalAbstractPath)
    {
        std::vector<std::shared_ptr<Node>> path;
        std::deque<std::shared_ptr<Edge>> queue;

        std::move( 
            hierarchicalAbstractPath.begin(),
            hierarchicalAbstractPath.end(),
            std::back_inserter(queue)
        );

        while (queue.size() > 0) {
            const auto& current = queue.front();
            queue.pop_front();

            if (current->underlyingPath.empty()) {
                path.push_back(current->start.lock());
                if (queue.empty()) {
                    path.push_back(current->end.lock());
                }
            } else {
                for (int i = current->underlyingPath.size() - 1; i >= 0; i--) {
                    queue.push_front(current->underlyingPath[i]);
                }
            }
        }

        return path;
    }

    /**
     * Преобразует высокоуровневый путь по вершинам абстрактного графа в путь по ячейкам карты.
     * @param abstractPath Абстрактный путь по вершинам графа.
     * @return Путь по ячейкам карты.
     */
    static std::vector<Point> getConcretePath(const std::vector<std::shared_ptr<Node>>& abstractPath,
                                              PathfindingGraph& graph,
                                              bool findNearestToTarget) {
        std::vector<Point> mapPath;
        for (int i = 1; i < abstractPath.size(); i++)
        {
            if (!mapPath.empty()) {
                mapPath.pop_back();
            }

            const std::shared_ptr<Node>& prev = abstractPath[i - 1];
            const std::shared_ptr<Node>& next = abstractPath[i];

            auto edge = prev->findEdge(next);
            if (edge != nullptr && edge->type == Edge::Type::PORTAL) {
                mapPath.push_back(prev->position);
                mapPath.push_back(next->position);
                continue;
            }

            if (edge != nullptr && !edge->underlyingMapPath.empty()) {
                std::copy(edge->underlyingMapPath.begin(), edge->underlyingMapPath.end(), std::back_inserter(mapPath));
                continue;
            }

            uint32_t dx = std::abs(prev->position.x - next->position.x);
            uint32_t dy = std::abs(prev->position.y - next->position.y);

            if (dx <= 1 && dy <= 1) {
                mapPath.push_back(prev->position);
                mapPath.push_back(next->position);
                continue;
            }

            const auto& cluster = graph.findCluster(prev->position, 0);
            std::vector<Point> pathPart = graph.pathfinder.findPath(prev->position, next->position, graph.map,
                    graph.object, graph.heuristic, nullptr, findNearestToTarget, cluster->boundaries);

            if (pathPart.empty()) {
                break;
            }

            std::copy(pathPart.begin(), pathPart.end(), std::back_inserter(mapPath));
        }

        return mapPath;
    }

    /**
     * Перестраивает путь так, чтобы оказаться ближе к недостижимой цели.
     * @param path Путь.
     * @param dest Целевая точка.
     * @param graph Граф поиска пути.
     */
    static void toNearest(std::vector<Point>& path, const Point dest, const PathfindingGraph& graph) {
        int32_t nearestIndex = -1;
        uint32_t nearestCost = std::numeric_limits<uint32_t>::max();

        for (int i = 0; i < path.size(); i++) {
            Point pathItem = path[i];
            uint32_t cost = graph.heuristic.calculate(pathItem, dest);
            if (cost < nearestCost)
            {
                nearestCost = cost;
                nearestIndex = i;
            }
        }

        // Обрезаем путь до позиции, которая находится ближе всего к цели.
        while (path.size() > nearestIndex + 1) {
            path.pop_back();
        }

        // Движемся в сторону цели.
        std::vector<Point> towardsDest = moveTowardsDest(path[path.size() - 1], dest, graph.map, graph.object, graph.heuristic);
        std::copy(towardsDest.begin(), towardsDest.end(), std::back_inserter(path));
    }

    /**
     * Простое движение в сторону цели до первого препятствия, которе нельзя обойти за один шаг.
     * @param start Начальная ячейка.
     * @param dest Конечная ячейка.
     * @param physics Карта.
     * @param object объект для которого рассчитывается путь.
     * @param heuristic Функция эвристической оценки расстояния.
     * @return Список с ячейками карты.
     */
    static std::vector<Point> moveTowardsDest(const Point start, const Point dest, const Map& map, const MapObject& object, const Heuristic heuristic)
    {
        std::vector<Point> path;
        auto startOpt = std::optional<Point>{start};

        while ((startOpt = moveToBetterPosition(startOpt.value(), dest, map, object, heuristic)).has_value()) {
            path.push_back(startOpt.value());
        }
        return path;
    }

    /**
     * Находит соседнюю ячейку, которая находится ближе к цели.
     * @param start Точка старта.
     * @param dest Целевая точка.
     * @param physics Карта.
     * @param object объект для которого рассчитывается путь.
     * @param heuristic Функция эвристической оценки расстояния.
     * @return Координаты ячейки, есть свободная клетка, которая находится ближе к цели, иначе - null.
     */
    static std::optional<Point> moveToBetterPosition(const Point start, const Point dest, const Map& map, const MapObject& object, Heuristic heuristic) {
        Point betterPosition = start;
        uint32_t cost = heuristic.calculate(start, dest);
        for (const Direction direction : map.directions()) {
            const Point neighbourPos = start + direction;

            if (map.out(neighbourPos) || !map.allowed(direction)) {
                continue;
            }

            uint32_t neighbourCost = heuristic.calculate(neighbourPos, dest);
            if (neighbourCost >= cost) {
                continue;
            }

            if (!map.canMove(object, start, direction)) {
                continue;
            }

            betterPosition = neighbourPos;
            cost = neighbourCost;
        }

        if (betterPosition == start) {
            return std::nullopt;
        } else {
            return std::optional<Point>{betterPosition};
        }
    }
};

} // namespace Engine