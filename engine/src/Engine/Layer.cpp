#include "Layer.hpp"

#include "ComponentManager.hpp"
#include "Coordinator.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "SystemManager.hpp"

#include "AIComponent.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.hpp"
#include "Light3DComponent.hpp"
#include "LocationComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "VelocityComponent.hpp"

#include "AISystem.hpp"
#include "CameraSystem.hpp"
#include "CollisionSystem.hpp"
#include "Light3DSystem.hpp"
#include "MoveSystem.hpp"
#include "PhysicsSystem.hpp"
#include "Render3DSystem.hpp"

namespace Engine {

Layer::Layer() {
    m_Coordinator.Init();

    m_Coordinator.RegisterComponent<PhysicsComponent>();
    m_Coordinator.RegisterComponent<VelocityComponent>();
    m_Coordinator.RegisterComponent<LocationComponent>();
    m_Coordinator.RegisterComponent<Render3DComponent>();
    m_Coordinator.RegisterComponent<CollisionComponent>();
    m_Coordinator.RegisterComponent<Light3DComponent>();
    m_Coordinator.RegisterComponent<AIComponent>();
    m_Coordinator.RegisterComponent<CameraComponent>();

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
        signature.set(m_Coordinator.GetComponentType<CollisionComponent>());
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<VelocityComponent>());

        m_Coordinator.RegisterSystem<CollisionSystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<AIComponent>());

        m_Coordinator.RegisterSystem<AISystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<Render3DComponent>());

        m_Coordinator.RegisterSystem<Render3DSystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());
        signature.set(m_Coordinator.GetComponentType<Light3DComponent>());

        m_Coordinator.RegisterSystem<Light3DSystem>(signature);
    }

    {
        Signature signature;
        signature.set(m_Coordinator.GetComponentType<CameraComponent>());
        signature.set(m_Coordinator.GetComponentType<LocationComponent>());

        m_Coordinator.RegisterSystem<CameraSystem>(signature);
    }
}

void Layer::update() { m_Coordinator.UpdateSystems(); }

void Layer::clear() { m_Coordinator.DestroyEntities(); }

} // namespace Engine