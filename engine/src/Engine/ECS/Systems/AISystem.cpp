#include "AISystem.hpp"
#include "AIComponent.hpp"
#include "Application.hpp"

namespace Engine {
const void AISystem::Update(ComponentManager &components) {
    auto &app = Application::get();

    for (const auto entity : m_Entities) {
        auto &ai = components.GetComponent<AIComponent>(entity);
        ai.tree->update();
    }
}

} // namespace Engine
