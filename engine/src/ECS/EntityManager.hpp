#pragma once

#include <array>
#include <cassert>
#include <deque>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.hpp"
#include "FlatDictionary.hpp"
#include "IteratorRange.hpp"

namespace Engine {

class EntityManager {
  public:
    using EntityRange = IteratorRange<std::array<Entity, c_MaxEntities>::const_iterator>;

    Entity CreateEntity(const std::string &name) {
        Entity id = m_NextId;
        m_NextId++;

        m_EntitySignature.emplace(id);
        m_EntityName.add(id, name);
        m_NameToEntity.add(name, id);

        return id;
    }

    void DestroyEntity(Entity entity) {
        m_EntitySignature.remove(entity);
        m_NameToEntity.remove(m_EntityName[entity]);
        m_EntityName.remove(entity);
    }

    Entity GetEntity(const std::string &name) const { return m_NameToEntity[name]; }

    bool HasEntity(Entity entity) const { return m_EntityName.hasKey(entity); }

    bool HasEntity(const std::string &name) const { return m_NameToEntity.hasKey(name); }

    void SetSignature(Entity entity, Signature signature) { m_EntitySignature[entity] = signature; }

    Signature GetSignature(Entity entity) { return m_EntitySignature[entity]; }

    std::vector<Entity> GetEntities() { return m_EntitySignature.keys(); }

    const std::string &GetName(Entity entity) { return m_EntityName.getValue(entity); }

  private:
    FlatDictionary<Entity, Signature> m_EntitySignature;
    FlatDictionary<Entity, std::string> m_EntityName;
    FlatDictionary<std::string, Entity> m_NameToEntity;

    Entity m_NextId = 1;
};

} // namespace Engine
