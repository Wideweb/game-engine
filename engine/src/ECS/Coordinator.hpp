#pragma once

#include <memory>
#include <unordered_map>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace Engine {

class Coordinator {
  public:
    void Init() {
        m_ComponentManager = std::make_unique<ComponentManager>();
        m_EntityManager = std::make_unique<EntityManager>();
        m_SystemManager = std::make_unique<SystemManager>();
    }

    Entity CreateEntity(const std::string &name) {
        return m_EntityManager->CreateEntity(name);
    }

    Entity GetEntity(const std::string &name) const {
        return m_EntityManager->GetEntity(name);
    }

    void DestroyEntity(Entity entity) {
        m_SystemManager->RemoveEntity(entity);
        m_ComponentManager->RemoveEntityComponents(entity);
        m_EntityManager->DestroyEntity(entity);
    }

    template <typename T> void RegisterComponent() {
        m_ComponentManager->RegisterComponent<T>();
    }

    template <typename T> void AddComponent(Entity entity, T component) {
        m_ComponentManager->AddComponent<T>(entity, component);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), true);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T> void RemoveComponent(Entity entity) {
        m_ComponentManager->RemoveComponent<T>(entity);

        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), false);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T> T &GetComponent(Entity entity) {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    template <typename T> ComponentType GetComponentType() {
        return m_ComponentManager->GetComponentType<T>();
    }

    template <typename T>
    const std::weak_ptr<ComponentArray<T>> GetComponentArray() {
        m_ComponentManager->GetComponentArray<T>();
    }

    template <typename T> void RegisterSystem(Signature signature) {
        m_SystemManager->RegisterSystem<T>(signature);
    }

    void UpdateSystems() { m_SystemManager->Update(*m_ComponentManager); }

  private:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
};

} // namespace Engine
