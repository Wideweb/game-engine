#pragma once

#include <vector>

#include "System.hpp"

namespace Engine {

class SystemManager {
  public:
    template <typename T, class... TArgs>
    void RegisterSystem(Signature signature, TArgs &&... args) {
        m_Systems.push_back(std::make_shared<T>(std::forward<TArgs>(args)...));
        m_Signatures.push_back(signature);
    }

    void Update(ComponentManager &components) const {
        for (const auto &system : m_Systems) {
            system->Update(components);
        }
    }

    void Attach(ComponentManager &components) const {
        for (const auto &system : m_Systems) {
            system->Attach(components);
        }
    }

    void RemoveEntity(Entity entity) {
        for (auto const &system : m_Systems) {
            system->m_Entities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        for (size_t i = 0; i < m_Systems.size(); i++) {
            auto const &system = m_Systems[i];
            auto const &systemSignature = m_Signatures[i];

            if ((entitySignature & systemSignature) == systemSignature) {
                system->m_Entities.insert(entity);
            } else {
                system->m_Entities.erase(entity);
            }
        }
    }

  private:
    std::vector<std::shared_ptr<System>> m_Systems;
    std::vector<Signature> m_Signatures;
};

} // namespace Engine
