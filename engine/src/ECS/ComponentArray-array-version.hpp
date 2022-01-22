#pragma once

#include <array>
#include <cassert>
#include <string>
#include <unordered_map>

#include "Entity.hpp"
#include "EventDispatcher.hpp"

namespace Engine {

class IComponentArray {
  public:
    virtual ~IComponentArray() = default;
    virtual void Remove(Entity entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
  public:
    EventDispatcher<Entity> beforeRemove$;

    void Add(Entity entity, T component) {
        assert(m_Size < c_MaxEntities && "Too many components");

        m_Components[m_Size] = component;
        m_Entities[m_Size] = entity;
        m_EntityToIndex[entity] = m_Size;
        ++m_Size;
    }

    T &Get(Entity entity) {
        const auto &it = m_EntityToIndex.find(entity);

        // assert(it != m_EntityToIndex.end() && "no component.");

        if (it == m_EntityToIndex.end()) {
            throw std::string("No component:") + std::string(typeid(T).name());
        }

        return m_Components[it->second];
    }

    bool Has(Entity entity) { return m_EntityToIndex.find(entity) != m_EntityToIndex.end(); }

    void Remove(Entity entity) {
        if (m_EntityToIndex.find(entity) == m_EntityToIndex.end()) {
            return;
        }

        beforeRemove$.dispatch(entity);

        const auto index = m_EntityToIndex[entity];
        --m_Size;
        m_Components[index] = m_Components[m_Size];
        m_Entities[index] = m_Entities[m_Size];
        m_EntityToIndex[m_Entities[index]] = index;
        m_EntityToIndex.erase(entity);
    }

    uint32_t size() { return m_Size; }

    std::array<T, c_MaxEntities> &components() { return m_Components; }

    std::array<Entity, c_MaxEntities> &entities() { return m_Entities; }

  private:
    std::array<T, c_MaxEntities> m_Components;
    std::array<Entity, c_MaxEntities> m_Entities;
    uint32_t m_Size = 0;

    std::unordered_map<Entity, size_t> m_EntityToIndex;
};

} // namespace Engine
