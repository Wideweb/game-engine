#include "LocationSystem.hpp"

#include "LocationComponent.hpp"

namespace Engine {

void LocationSystem::Update(ComponentManager &components) const {
    for (const auto entity : m_Entities) {
        auto &location = components.GetComponent<LocationComponent>(entity);
        location.updated = false;
    }
}

} // namespace Engine
