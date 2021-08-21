#pragma once

#include <array>
#include <cassert>
#include <deque>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.hpp"
#include "IteratorRange.hpp"

namespace Engine {

class EntityManager {
  public:
    using EntityRange = IteratorRange<std::array<Entity, c_MaxEntities>::const_iterator>;

    EntityManager() {
        for (Entity i = 0; i < c_MaxEntities; i++) {
            m_Entities[i] = i + 1;
            m_EntityToIndex[i + 1] = i;
        }
    }

    Entity CreateEntity(const std::string &name) {
        assert(m_ActiveEntities < c_MaxEntities && "Too many entities");

        Entity id = m_Entities[m_ActiveEntities];

        m_NameToEntity[name] = id;
        m_EntityToName[id] = name;
        m_EntityToIndex[id] = m_ActiveEntities;

        ++m_ActiveEntities;
        return id;
    }

    void DestroyEntity(Entity entity) {
        size_t index = m_EntityToIndex[entity];
        const auto &name = m_EntityToName[entity];

        m_EntityToName.erase(entity);
        m_NameToEntity.erase(name);
        m_EntityToIndex.erase(entity);

        --m_ActiveEntities;

        Entity last = m_Entities[m_ActiveEntities];
        m_Entities[index] = last;
        m_EntityToIndex[last] = index;
        m_Entities[m_ActiveEntities] = entity;
    }

    Entity GetEntity(const std::string &name) const {
        const auto &it = m_NameToEntity.find(name);

        assert(it != m_NameToEntity.end() && "no entity.");

        return it->second;
    }

    bool HasEntity(const std::string &name) const { return m_NameToEntity.find(name) != m_NameToEntity.end(); }

    void SetSignature(Entity entity, Signature signature) { m_Signatures[entity] = signature; }

    Signature GetSignature(Entity entity) { return m_Signatures[entity]; }

    std::vector<Entity> GetEntities() { return {m_Entities.begin(), m_Entities.begin() + m_ActiveEntities}; }

    const std::string &GetName(Entity entity) { return m_EntityToName.at(entity); }

  private:
    std::array<Entity, c_MaxEntities> m_Entities;
    std::array<Signature, c_MaxEntities> m_Signatures;
    std::unordered_map<std::string, Entity> m_NameToEntity;
    std::unordered_map<Entity, std::string> m_EntityToName;
    std::unordered_map<Entity, size_t> m_EntityToIndex;

    uint32_t m_ActiveEntities = 0;
};

} // namespace Engine
