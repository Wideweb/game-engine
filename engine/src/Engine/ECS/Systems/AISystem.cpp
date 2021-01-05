#include "AISystem.hpp"
#include "AIComponent.hpp"
#include "Application.hpp"

namespace Engine {
void AISystem::Update(ComponentManager &components) const {
    for (const auto entity : m_Entities) {
        auto &ai = components.GetComponent<AIComponent>(entity);
        ai.tree->update();
    }
}

} // namespace Engine
