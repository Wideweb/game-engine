#pragma once

#include <string>

#include "Entity.hpp"
#include "EventDispatcher.hpp"
#include "FlatDictionary.hpp"

namespace Engine {

class IComponentArray {
  public:
    virtual ~IComponentArray() = default;
    virtual void Remove(Entity entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
  public:
    EventDispatcher<Entity> beforeRemove$;

    void Add(Entity entity, T component) { m_Components.add(entity, component); }

    T &Get(Entity entity) {
        if (!Has(entity)) {
            throw std::string("No component:") + std::string(typeid(T).name());
        }

        return m_Components[entity];
    }

    bool Has(Entity entity) { return m_Components.hasKey(entity); }

    void Remove(Entity entity) {
        if (!Has(entity)) {
            return;
        }

        beforeRemove$.dispatch(entity);

        m_Components.remove(entity);
    }

    uint32_t size() { return m_Components.size(); }

    std::vector<T> &components() { return m_Components.values(); }

    std::vector<Entity> &entities() { return m_Components.keys(); }

  private:
    FlatDictionary<Entity, T> m_Components;
};

} // namespace Engine
