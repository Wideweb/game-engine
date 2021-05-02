#include "Layer.hpp"

#include "ComponentManager.hpp"
#include "Coordinator.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "SystemManager.hpp"

#include "AIComponent.hpp"
#include "BehaviourComponent.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.hpp"
#include "Light3DComponent.hpp"
#include "LocationComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "StaticRender3DComponent.hpp"
#include "TerrainCollisionComponent.hpp"
#include "VelocityComponent.hpp"

#include "AISystem.hpp"
#include "BehaviourSystem.hpp"
#include "CameraSystem.hpp"
#include "CollisionSystem.hpp"
#include "Light3DSystem.hpp"
#include "MoveSystem.hpp"
#include "PhysicsSystem.hpp"
#include "Render3DSystem.hpp"
#include "SkeletSystem.hpp"
#include "StaticCollisionSystem.hpp"
#include "StaticRender3DSystem.hpp"
#include "TerrainCollisionSystem.hpp"

namespace Engine {

Layer::Layer(std::string name) : m_Name(std::move(name)) {
    m_Coordinator.RegisterComponent<PhysicsComponent>();
    m_Coordinator.RegisterComponent<VelocityComponent>();
    m_Coordinator.RegisterComponent<LocationComponent>();
    m_Coordinator.RegisterComponent<StaticRender3DComponent>();
    m_Coordinator.RegisterComponent<Render3DComponent>();
    m_Coordinator.RegisterComponent<SkeletComponent>();
    m_Coordinator.RegisterComponent<StaticCollisionComponent>();
    m_Coordinator.RegisterComponent<TerrainCollisionComponent>();
    m_Coordinator.RegisterComponent<CollisionComponent>();
    m_Coordinator.RegisterComponent<Light3DComponent>();
    m_Coordinator.RegisterComponent<AIComponent>();
    m_Coordinator.RegisterComponent<CameraComponent>();
    m_Coordinator.RegisterComponent<BehaviourComponent>();

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<PhysicsComponent>());
        signature.set(m_Coordinator.GetComponentType<VelocityComponent>());

        m_Coordinator.RegisterSystem<PhysicsSystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<VelocityComponent>());

        m_Coordinator.RegisterSystem<MoveSystem>(signature);
    }

    {
        Signature signature;
        signature.set(
            m_Coordinator.GetComponentType<StaticCollisionComponent>());
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());

        m_Coordinator.RegisterSystem<StaticCollisionSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(
            m_Coordinator.GetComponentType<TerrainCollisionComponent>());
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(
            m_Coordinator.GetComponentType<StaticRender3DComponent>());

        m_Coordinator.RegisterSystem<TerrainCollisionSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<CollisionComponent>());
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<VelocityComponent>());

        m_Coordinator.RegisterSystem<CollisionSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<AIComponent>());

        m_Coordinator.RegisterSystem<AISystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(
            m_Coordinator.GetComponentType<StaticRender3DComponent>());

        m_Coordinator.RegisterSystem<StaticRender3DSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<Render3DComponent>());

        m_Coordinator.RegisterSystem<Render3DSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<SkeletComponent>());
        signature.set(m_Coordinator.GetComponentType<Render3DComponent>());

        m_Coordinator.RegisterSystem<SkeletSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<Light3DComponent>());

        m_Coordinator.RegisterSystem<Light3DSystem>(signature, m_Name);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<CameraComponent>());
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());

        m_Coordinator.RegisterSystem<CameraSystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<BehaviourComponent>());

        m_Coordinator.RegisterSystem<BehaviourSystem>(signature, m_Name);
    }
}

void Layer::attach() {
    onAttach();
    m_Active = true;
}

void Layer::update() {
    m_Coordinator.UpdateSystems();
    onUpdate();
}

void Layer::detach() {
    onDetach();
    m_Active = false;
}

} // namespace Engine