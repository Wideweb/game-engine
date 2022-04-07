#include "ParentSystem.hpp"

#include "Application.hpp"
#include "ParentComponent.hpp"

namespace Engine {

void ParentSystem::Attach(ComponentManager &components) const {
    components.GetComponentArray<ParentComponent>()->beforeRemove$.addEventCallback([this](Entity entity) {
        auto &parent = getCoordinator().GetComponent<ParentComponent>(entity);
        if (parent.entity != c_NoEntity) {
            m_EntityChildren[parent.entity].erase(entity);
        }
    });
}

void ParentSystem::Update(ComponentManager &components) const {
    size_t toRemoveIndex = 0;

    for (const auto entity : m_Entities) {
        auto &parent = components.GetComponent<ParentComponent>(entity);

        if (parent.entity != c_NoEntity && !getCoordinator().HasEntity(parent.entity)) {
            m_ToRemove[toRemoveIndex++] = entity;
        }

        if (parent.isDirty) {
            if (parent.prevEntity != c_NoEntity) {
                m_EntityChildren[parent.prevEntity].erase(entity);
            }

            if (parent.entity != c_NoEntity) {
                m_EntityChildren[parent.entity].insert(entity);
            }
            parent.isDirty = false;
        }
    }

    for (size_t i = 0; i < toRemoveIndex; i++) {
        RemoveNode(components, m_ToRemove[i]);
    }
}

void ParentSystem::RemoveNode(ComponentManager &components, Entity entity) const {
    auto &parent = components.GetComponent<ParentComponent>(entity);
    if (!parent.destroyWithParent) {
        parent.setEntity(c_NoEntity);
        return;
    }

    if (m_EntityChildren.find(entity) != m_EntityChildren.end()) {
        auto &children = m_EntityChildren[entity];
        for (auto child : children) {
            RemoveNode(components, child);
        }
        m_EntityChildren.erase(entity);
    }

    getCoordinator().DestroyEntity(entity);
}

} // namespace Engine
