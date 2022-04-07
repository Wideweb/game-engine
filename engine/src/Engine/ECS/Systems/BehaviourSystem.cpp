#include "BehaviourSystem.hpp"

#include "Application.hpp"
#include "BehaviourComponent.hpp"
#include "CollisionComponent.hpp"
#include "EntityScript.hpp"

#include <memory>

namespace Engine {

void BehaviourSystem::Update(ComponentManager &components) const {
    if (Application::get().getTime().poused()) {
        return;
    }

    auto &layer = getLayer();
    auto &scripts = getScripts();
    auto &eventHandler = Application::get().getEventHandler();

    for (const auto entity : m_Entities) {
        auto &behaviour = components.GetComponent<BehaviourComponent>(entity);

        if (behaviour.invalid || behaviour.script.empty()) {
            continue;
        }

        auto script = scripts.get<EntityScript>(entity);
        if (script != nullptr) {
            try {
                script->update();

                if (components.HasComponent<CollisionComponent>(entity)) {
                    auto &collision = components.GetComponent<CollisionComponent>(entity);
                    for (auto otherEntity : collision.entities) {
                        m_LuaEntity.setLayer(&layer);
                        m_LuaEntity.setEntity(otherEntity);
                        script->collide(m_LuaEntity);
                    }
                }

            } catch (const std::invalid_argument &e) {
                behaviour.invalid = true;
                eventHandler.send<ApplicationError>(ApplicationError(e.what()));
            }

            continue;
        }

        LuaEntity luaEntity{entity, &layer};
        scripts.add(entity, std::make_shared<EntityScript>(behaviour.script, luaEntity));
    }
}

} // namespace Engine
