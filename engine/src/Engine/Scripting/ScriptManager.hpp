#pragma once

#include "Entity.hpp"
#include "Script.hpp"

#include <memory>
#include <unordered_map>
#include <utility>

namespace Engine {

class ScriptManager {
  public:
    void add(Entity entity, std::shared_ptr<ScriptBase> script) {
        m_EntityScript[entity] = script;
    }

    template <typename TScript, typename = std::enable_if_t<
                                    std::is_base_of_v<ScriptBase, TScript>>>
    const std::shared_ptr<TScript> get(Entity entity) {
        auto it = m_EntityScript.find(entity);
        if (it == m_EntityScript.end()) {
            return nullptr;
        }

        return std::dynamic_pointer_cast<TScript>(it->second);
    }

  private:
    std::unordered_map<Entity, std::shared_ptr<ScriptBase>> m_EntityScript;
};

} // namespace Engine
