#pragma once

#include "Point.hpp"
#include "Map.hpp"
#include "MapObject.hpp"
#include "Heuristic.hpp"
#include "PathSmoother.hpp"
#include "Boundaries.hpp"
#include "PathFinderResultHandler.hpp"

#include <vector>
#include <memory>
#include <optional>

namespace Engine {

/**
 * Используется для поиска пути по ячейкам игровой карты.
 */
class PathfinderConcrete {
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
    virtual void findPath(const Point start,
                          const Point dest,
                          const Map& map,
                          const MapObject& object,
                          const Heuristic heuristic,
                          const std::shared_ptr<PathSmoother>& smoother,
                          bool findNearestToTarget,
                          const std::optional<Boundaries> boundaries,
                          const std::shared_ptr<PathFinderResultHandler>& handler) = 0;
};

} // namespace Engine
