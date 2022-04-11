#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace Engine {

class Coordinator {
  public:
    Coordinator() {
        m_ComponentManager = std::make_unique<ComponentManager>();
        m_EntityManager = std::make_unique<EntityManager>();
        m_SystemManager = std::make_unique<SystemManager>();
    }

    Entity CreateEntity(const std::string &name) { return m_EntityManager->CreateEntity(name); }

    Entity GetEntity(const std::string &name) const { return m_EntityManager->GetEntity(name); }

    bool HasEntity(Entity entity) const { return m_EntityManager->HasEntity(entity); }

    bool HasEntity(const std::string &name) const { return m_EntityManager->HasEntity(name); }

    void DestroyEntity(Entity entity) {
        m_SystemManager->RemoveEntity(entity);
        m_ComponentManager->RemoveEntityComponents(entity);
        m_EntityManager->DestroyEntity(entity);
    }

    std::vector<Entity> GetEntities() { return m_EntityManager->GetEntities(); }

    const std::string &GetEntityName(Entity entity) { return m_EntityManager->GetName(entity); }

    bool ChangeEntityName(Entity entity, std::string name) { return m_EntityManager->ChangeName(entity, name); }

    template <typename T> void RegisterComponent() { m_ComponentManager->RegisterComponent<T>(); }

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

    template <typename T> void SetComponentActive(Entity entity, bool isActive) {
        auto signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), isActive);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);

        m_ComponentManager->GetComponentArray<T>()->active$.dispatch(entity, isActive);
    }

    template <typename T> bool IsComponentActive(Entity entity) {
        return m_EntityManager->GetSignature(entity).test(m_ComponentManager->GetComponentType<T>());
    }

    template <typename T> T &GetComponent(Entity entity) { return m_ComponentManager->GetComponent<T>(entity); }

    template <typename T> bool HasComponent(Entity entity) { return m_ComponentManager->HasComponent<T>(entity); }

    template <typename T> ComponentType GetComponentType() { return m_ComponentManager->GetComponentType<T>(); }

    template <typename T> const std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        return m_ComponentManager->GetComponentArray<T>();
    }

    template <typename T, class... TArgs> void RegisterSystem(Signature signature, TArgs &&...args) {
        m_SystemManager->RegisterSystem<T>(signature, std::forward<TArgs>(args)...);
    }

    void UpdateSystems() { m_SystemManager->Update(*m_ComponentManager); }

    void AttachSystems() { m_SystemManager->Attach(*m_ComponentManager); }

    void Clear() {
        auto entitites = m_EntityManager->GetEntities();
        for (auto &entity : entitites) {
            DestroyEntity(entity);
        }
    }

    ComponentManager &GetComponentManager() { return *m_ComponentManager; }

  private:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
};

} // namespace Engine
