#include "EntityManager.hpp"

namespace Engine {

Entity EntityManager::s_NextId = 1;

Entity EntityManager::CreateEntity(const std::string &name) {
    Entity id = s_NextId;
    s_NextId++;

    m_EntitySignature.emplace(id);
    m_EntityName.add(id, name);
    m_NameToEntity.add(name, id);

    return id;
}

void EntityManager::DestroyEntity(Entity entity) {
    m_EntitySignature.remove(entity);
    m_NameToEntity.remove(m_EntityName[entity]);
    m_EntityName.remove(entity);
}

Entity EntityManager::GetEntity(const std::string &name) const { return m_NameToEntity[name]; }

bool EntityManager::HasEntity(Entity entity) const { return m_EntityName.hasKey(entity); }

bool EntityManager::HasEntity(const std::string &name) const { return m_NameToEntity.hasKey(name); }

void EntityManager::SetSignature(Entity entity, Signature signature) { m_EntitySignature[entity] = signature; }

Signature EntityManager::GetSignature(Entity entity) { return m_EntitySignature[entity]; }

std::vector<Entity> EntityManager::GetEntities() { return m_EntitySignature.keys(); }

const std::string &EntityManager::GetName(Entity entity) { return m_EntityName.getValue(entity); }

} // namespace Engine
