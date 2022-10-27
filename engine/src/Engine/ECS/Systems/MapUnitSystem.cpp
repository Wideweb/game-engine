#include "MapUnitSystem.hpp"

#include "MapUnitComponent.hpp"
#include "Application.hpp"
#include "HierarchicalPathfinder.hpp"

#include <memory>

namespace Engine {

void MapUnitSystem::Update(ComponentManager &components) const {
    auto &map = Application::get().getMap();
    auto &graph = Application::get().getPathfindingGraph();

    for (const auto entity : m_Entities) {
        auto &unit = components.GetComponent<MapUnitComponent>(entity);

        if (unit.target == unit.mapObject.position) {
            continue;
        }

        if (unit.path.size() > 0 && unit.path[unit.path.size() - 1] == unit.target) {
            // move();
            continue;
        }

        HierarchicalPathfinder::findPath(graph, Point(0, 0), Point(5, 5), nullptr, false, std::static_pointer_cast<HPathFinderResultHandler>(m_ResultHandler));
        unit.pathIndex = 0;

        if (m_ResultHandler->path.empty()) {
            unit.target = unit.mapObject.position;
            continue;
        } 

        unit.path = std::move(m_ResultHandler->path);
    }
}

} // namespace Engine
