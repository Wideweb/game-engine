#pragma once

#include <array>
#include <cassert>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.hpp"
#include "IteratorRange.hpp"

namespace Engine {

class EntityManager {
  public:
    using EntityRange =
        IteratorRange<std::array<Entity, c_MaxEntities>::const_iterator>;

    EntityManager() {
        for (Entity i = 0; i < c_MaxEntities; i++) {
            m_Entities.push(i);
        }
    }

    Entity CreateEntity(const std::string &name) {
        assert(m_ActiveEntities < c_MaxEntities && "Too many entities");

        Entity id = m_Entities.front();
        m_Entities.pop();

        m_NameToEntity[name] = id;
        m_EntityToName[id] = name;

        ++m_ActiveEntities;
        return id;
    }

    void DestroyEntity(Entity entity) {
        const auto &name = m_EntityToName[entity];

        m_EntityToName.erase(entity);
        m_NameToEntity.erase(name);

        m_Entities.push(entity);
        --m_ActiveEntities;
    }

    Entity GetEntity(const std::string &name) const {
        const auto &it = m_NameToEntity.find(name);

        assert(it != m_NameToEntity.end() && "no entity.");

        return it->second;
    }

    void SetSignature(Entity entity, Signature signature) {
        m_Signatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) { return m_Signatures[entity]; }

    std::vector<Entity> GetEntities() {
        std::vector<Entity> entitites;
        entitites.resize(m_ActiveEntities);
        std::iota(entitites.begin(), entitites.end(), 0);
        return entitites;
    }

  private:
    std::queue<Entity> m_Entities;
    std::array<Signature, c_MaxEntities> m_Signatures;
    std::unordered_map<std::string, Entity> m_NameToEntity;
    std::unordered_map<Entity, std::string> m_EntityToName;

    uint32_t m_ActiveEntities = 0;
};

} // namespace Engine
