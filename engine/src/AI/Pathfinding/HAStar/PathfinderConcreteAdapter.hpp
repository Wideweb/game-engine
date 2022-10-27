#pragma once

#include "Point.hpp"
#include "Map.hpp"
#include "MapObject.hpp"
#include "Heuristic.hpp"
#include "PathSmoother.hpp"
#include "Boundaries.hpp"
#include "PathFinderResultHandler.hpp"
#include "PathfinderConcrete.hpp"

#include <vector>
#include <memory>
#include <optional>

namespace Engine {

class PathfinderConcreteAdapter {
  private:
    class PathResultHandler : public PathFinderResultHandler {
      public:
        std::vector<Point> path;

        void onPathFound(const std::vector<Point>& path) override {
            this->path = path;
        }

        void onPathNotFound() override {
            this->path.clear();
        }
    };

    std::shared_ptr<PathResultHandler> m_PathResultHandler;

    std::shared_ptr<PathfinderConcrete> m_Pathfinder;

  public:
    PathfinderConcreteAdapter() {}

    PathfinderConcreteAdapter(const std::shared_ptr<PathfinderConcrete>& pathfinder) : m_Pathfinder (pathfinder) {
        m_PathResultHandler = std::make_shared<PathResultHandler>();
    }

    std::vector<Point> findPath(const Point start,
                                const Point dest,
                                const Map& map,
                                const MapObject& object,
                                const Heuristic heuristic,
                                const std::shared_ptr<PathSmoother> smoother,
                                bool findNearestToTarget,
                                const std::optional<Boundaries> boundaries) {

        m_Pathfinder->findPath(start, dest, map, object, heuristic, smoother, findNearestToTarget, boundaries,
                                    std::static_pointer_cast<PathFinderResultHandler>(m_PathResultHandler));
        return m_PathResultHandler->path;

    }
};

} // namespace Engine
