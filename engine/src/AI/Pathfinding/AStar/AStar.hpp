#pragma once

#include "AStarNode.hpp"
#include "Point.hpp"
#include "Direction.hpp"
#include "Map.hpp"
#include "MapObject.hpp"
#include "PathSmoother.hpp"
#include "Heuristic.hpp"
#include "Boundaries.hpp"
#include "BalancedBinaryHeap.hpp"
#include "SpatialMap.hpp"
#include "PathFinderResultHandler.hpp"
#include "PathfinderConcrete.hpp"

#include <array>
#include <memory>
#include <optional>

namespace Engine {

class AStar : public PathfinderConcrete {
  private:
    SpatialMap<AStarNode, 8> spatialMap;

  public:
    /**
     * Находит оптимальный путь из одной ячейки карты в другую в пределах области.
     * @param start Начальная ячейка.
     * @param dest Конечная ячейка.
     * @param map Карта.
     * @param object объект для которого рассчитывается путь.
     * @param heuristic Функция эвристической оценки расстояния.
     * @param smoother Алгоритм сглаживания пути.
     * @param findNearestToTarget Флаг, отвечающий за поиск ближайшей точки к цели, если не удалось найти завершенный путь.
     * @param boundaries Область, ограничивающая пространство поиска.
     * @param handler Обработчик результатов поиска.
     */
    void findPath(const Point start,
                  const Point dest,
                  const Map& map,
                  const MapObject& object,
                  const Heuristic heuristic,
                  const std::shared_ptr<PathSmoother>& smoother,
                  bool findNearestToTarget,
                  const std::optional<Boundaries> boundaries,
                  const std::shared_ptr<PathFinderResultHandler>& handler) override {
        
        spatialMap.init(map.height, map.width);
        AStarNode* startNode = spatialMap.emplaceNode(start.x, start.y, start.x, start.y, heuristic.calculate(start, dest));

        BalancedBinaryHeap<AStarNode*> openSet;
        openSet.add(startNode);

        while (!openSet.empty()) {
            AStarNode* currentNode = openSet.poll();
            Point currentPos = Point(currentNode->x, currentNode->y);

            if (currentPos == dest) {
                handler->onPathFound(smooth(reconstructPath(currentNode), smoother));
                spatialMap.clear();
                return;
            }

            for (const Direction direction : map.directions()) {
                Point childPos = currentPos + direction;

                if (boundaries.has_value() && !boundaries.value().contains(childPos)) {
                    continue;
                }

                int childG = currentNode->g + (direction.alongAxes() ? Heuristic::D : Heuristic::D2);

                if (map.out(childPos)) {
                    continue;
                }

                if (!map.allowed(direction)) {
                    continue;
                }

                if (!map.canMove(object, currentPos, direction)) {
                    continue;
                }

                bool childNodeExists = spatialMap.hasNode(childPos.x, childPos.y);
                if (!childNodeExists) {
                    spatialMap.emplaceNode(childPos.x, childPos.y, childPos.x, childPos.y, heuristic.calculate(childPos, dest));
                }

                AStarNode* childNode = spatialMap.getNodePtr(childPos.x, childPos.y);

                if (childNodeExists && childG >= childNode->g) {
                    continue;
                }

                childNode->parent = currentNode;
                childNode->g = childG;

                if (openSet.contains(childNode)) {
                    openSet.update(childNode);
                } else {
                    openSet.add(childNode);
                }
            }
        }

        if (findNearestToTarget) {
            AStarNode closestNode = AStarNode(startNode->x, startNode->y);
            closestNode.parent = startNode->parent;
            uint32_t min = std::numeric_limits<uint32_t>::max();

            spatialMap.forEach([dest, &closestNode, &min](AStarNode& node){
                int32_t x = node.x;
                int32_t y = node.y;
                int32_t d = (node.x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y);
                if (d < min)
                {
                    closestNode.parent = node.parent;
                    closestNode.x = x;
                    closestNode.y = y;
                    min = d;
                }
            });

            handler->onPathFound(smooth(reconstructPath(&closestNode), smoother));
        } else {
            handler->onPathNotFound();
        }

        spatialMap.clear();
    }

  private:

    /**
     * Построит путь. Для этого пройдется по цепочке child -> parent, начиная
     * с указанного узла.
     * @param goalNode Целевой узел.
     * @return Путь.
     */
    std::vector<Point> reconstructPath(AStarNode* goalNode) {
        std::vector<Point> path;

        AStarNode* current = goalNode;
        while (current != nullptr) {
            path.emplace_back(current->x, current->y);
            current = current->parent;
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    /**
     * Сгладит переданный путь.
     * @param path Путь,
     * @param smoother Алгоритм сглаживания пути.
     * @return Сглаженный путь.
     */
    std::vector<Point> smooth(const std::vector<Point>& path, const std::shared_ptr<PathSmoother>& smoother) {
        if (smoother != nullptr) {
            return smoother->smooth(path);
        }
        return path;
    }
};

} // namespace Engine
