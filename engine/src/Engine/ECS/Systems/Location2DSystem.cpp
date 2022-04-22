#include "Location2DSystem.hpp"

#include "Location2DComponent.hpp"

namespace Engine {

void Location2DSystem::Update(ComponentManager &components) const {
    for (const auto entity : m_Entities) {
        auto &location = components.GetComponent<Location2DComponent>(entity);
        location.updated = false;
    }
}

} // namespace Engine
