#include "BehaviourSystem.hpp"

#include "Application.hpp"
#include "BehaviourComponent.hpp"
#include "EntityScript.hpp"
#include "LuaEntity.hpp"

#include <memory>

namespace Engine {

void BehaviourSystem::Update(ComponentManager &components) const {
    auto &layer = getLayer();
    auto &scripts = getScripts();

    for (const auto entity : m_Entities) {
        auto &behaviour = components.GetComponent<BehaviourComponent>(entity);

        auto script = scripts.get<EntityScript>(entity);
        if (script != nullptr) {
            script->update();
            continue;
        }

        LuaEntity luaEntity{entity, &layer};
        scripts.add(entity, std::make_shared<EntityScript>(behaviour.script,
                                                           luaEntity));
    }
}

} // namespace Engine
