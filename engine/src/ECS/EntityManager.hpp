#pragma once

#include <array>
#include <numeric>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "FlatDictionary.hpp"
#include "IteratorRange.hpp"

namespace Engine {

class EntityManager {
  public:
    using EntityRange = IteratorRange<std::array<Entity, c_MaxEntities>::const_iterator>;

    Entity CreateEntity(const std::string &name);

    void DestroyEntity(Entity entity);

    Entity GetEntity(const std::string &name) const;

    bool HasEntity(Entity entity) const;

    bool HasEntity(const std::string &name) const;

    void SetSignature(Entity entity, Signature signature);

    Signature GetSignature(Entity entity);

    std::vector<Entity> GetEntities();

    const std::string &GetName(Entity entity);

  private:
    FlatDictionary<Entity, Signature> m_EntitySignature;
    FlatDictionary<Entity, std::string> m_EntityName;
    FlatDictionary<std::string, Entity> m_NameToEntity;

    static Entity s_NextId;
};

} // namespace Engine
