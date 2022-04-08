#include "DestroySystem.hpp"
#include "DestroyComponent.hpp"

namespace Engine {

void DestroySystem::Update(ComponentManager &components) const {
    size_t toRemoveIndex = 0;

    for (const auto entity : m_Entities) {
        auto &destroy = components.GetComponent<DestroyComponent>(entity);
        if (destroy.cycles == 0) {
            m_ToRemove[toRemoveIndex++] = entity;
        } else {
            destroy.cycles--;
        }
    }

    for (size_t i = 0; i < toRemoveIndex; i++) {
        getCoordinator().DestroyEntity(m_ToRemove[i]);
    }
}

} // namespace Engine
