#include "LuaEntity.hpp"

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
#include "VelocityComponent.hpp"

namespace Engine {

void LuaEntity::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginClass<LuaEntity>("Entity")
        .addFunction("getLocationComponent",
                     &LuaEntity::getComponent<LocationComponent>)
        .addFunction("addLocationComponent",
                     &LuaEntity::addComponent<LocationComponent>)
        .addFunction("getVelocityComponent",
                     &LuaEntity::getComponent<VelocityComponent>)
        .addFunction("addVelocityComponent",
                     &LuaEntity::addComponent<VelocityComponent>)
        .addFunction("addRender3DComponent",
                     &LuaEntity::addComponent<Render3DComponent>)
        .addFunction("addStaticRender3DComponent",
                     &LuaEntity::addComponent<StaticRender3DComponent>)
        .addFunction("addCollisionComponent",
                     &LuaEntity::addComponent<CollisionComponent>)
        .addFunction("addStaticCollisionComponent",
                     &LuaEntity::addComponent<StaticCollisionComponent>)
        .addFunction("addLight3DComponent",
                     &LuaEntity::addComponent<Light3DComponent>)
        .addFunction("addPhysicsComponent",
                     &LuaEntity::addComponent<PhysicsComponent>)
        .addFunction("addCameraComponent",
                     &LuaEntity::addComponent<CameraComponent>)
        .addFunction("addBehaviourComponent",
                     &LuaEntity::addComponent<BehaviourComponent>)
        .addFunction("addSkeletComponent",
                     &LuaEntity::addComponent<SkeletComponent>)
        .endClass();
}

LuaEntity::LuaEntity(Entity entity, Layer *layer)
    : m_Entity(entity), m_Layer(layer) {}

} // namespace Engine