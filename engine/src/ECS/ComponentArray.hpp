#pragma once

#include <array>
#include <cassert>
#include <unordered_map>

#include "Entity.hpp"

namespace Engine {

class IComponentArray {
  public:
    virtual ~IComponentArray() = default;
    virtual void Remove(Entity entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
  public:
    void Add(Entity entity, T component) {
        assert(m_Size < c_MaxEntities && "Too many components");

        m_Components[m_Size] = component;
        m_EntityToIndex[entity] = m_Size;
        ++m_Size;
    }

    T &Get(Entity entity) {
        const auto &it = m_EntityToIndex.find(entity);

        assert(it != m_EntityToIndex.end() && "no component.");

        return m_Components[it->second];
    }

    void Remove(Entity entity) {
        if (m_EntityToIndex.find(entity) == m_EntityToIndex.end()) {
            return;
        }

        const auto index = m_EntityToIndex[entity];
        --m_Size;
        m_EntityToIndex[index] = m_EntityToIndex[m_Size];
    }

  private:
    std::array<T, c_MaxEntities> m_Components;
    uint32_t m_Size = 0;

    std::unordered_map<Entity, size_t> m_EntityToIndex;
};

} // namespace Engine
