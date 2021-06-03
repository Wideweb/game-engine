#pragma once

#include <memory>
#include <typeinfo>
#include <unordered_map>

#include "ComponentArray.hpp"
#include "Entity.hpp"

namespace Engine {

class ComponentManager {
  public:
    template <typename T> void RegisterComponent() {
        const auto *key = &typeid(T);

        m_Components[key] = std::make_shared<ComponentArray<T>>();
        m_ComponentType[key] = m_NextComponentType;

        ++m_NextComponentType;
    }

    template <typename T> void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->Add(entity, component);
    }

    template <typename T> T &GetComponent(Entity entity) { return GetComponentArray<T>()->Get(entity); }

    template <typename T> bool HasComponent(Entity entity) { return GetComponentArray<T>()->Has(entity); }

    template <typename T> ComponentType GetComponentType() {
        const auto *key = &typeid(T);

        return m_ComponentType[key];
    }

    template <typename T> void RemoveComponent(Entity entity) { GetComponentArray<T>()->Remove(entity); }

    void RemoveEntityComponents(Entity entity) {
        for (auto const &pair : m_Components) {
            pair.second->Remove(entity);
        }
    }

    template <typename T> std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        const auto *key = &typeid(T);

        return std::static_pointer_cast<ComponentArray<T>>(m_Components[key]);
    }

  private:
    std::unordered_map<const std::type_info *, std::shared_ptr<IComponentArray>> m_Components;
    std::unordered_map<const std::type_info *, ComponentType> m_ComponentType;

    ComponentType m_NextComponentType = 0;
};

} // namespace Engine
