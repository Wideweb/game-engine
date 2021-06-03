#include "LuaEntity.hpp"

#include "AIComponent.hpp"
#include "BehaviourComponent.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.hpp"
#include "DirectedLightComponent.hpp"
#include "LocationComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "SpotLightComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "StaticRender3DComponent.hpp"
#include "TerrainCollisionComponent.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

void LuaEntity::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginClass<LuaEntity>("Entity")
        .addFunction("getLocationComponent", &LuaEntity::getComponent<LocationComponent>)
        .addFunction("addLocationComponent", &LuaEntity::addComponent<LocationComponent>)
        .addFunction("getVelocityComponent", &LuaEntity::getComponent<VelocityComponent>)
        .addFunction("addVelocityComponent", &LuaEntity::addComponent<VelocityComponent>)
        .addFunction("addRender3DComponent", &LuaEntity::addComponent<Render3DComponent>)
        .addFunction("addStaticRender3DComponent", &LuaEntity::addComponent<StaticRender3DComponent>)
        .addFunction("getCollisionComponent", &LuaEntity::getComponent<CollisionComponent>)
        .addFunction("addCollisionComponent", &LuaEntity::addComponent<CollisionComponent>)
        .addFunction("addTerrainCollisionComponent", &LuaEntity::addComponent<TerrainCollisionComponent>)
        .addFunction("getStaticCollisionComponent", &LuaEntity::getComponent<StaticCollisionComponent>)
        .addFunction("addStaticCollisionComponent", &LuaEntity::addComponent<StaticCollisionComponent>)
        .addFunction("addDirectedLightComponent", &LuaEntity::addComponent<DirectedLightComponent>)
        .addFunction("addSpotLightComponent", &LuaEntity::addComponent<SpotLightComponent>)
        .addFunction("addPhysicsComponent", &LuaEntity::addComponent<PhysicsComponent>)
        .addFunction("addCameraComponent", &LuaEntity::addComponent<CameraComponent>)
        .addFunction("addBehaviourComponent", &LuaEntity::addComponent<BehaviourComponent>)
        .addFunction("getSkeletComponent", &LuaEntity::getComponent<SkeletComponent>)
        .addFunction("addSkeletComponent", &LuaEntity::addComponent<SkeletComponent>)
        .addFunction("getId", &LuaEntity::getId)
        .endClass();
}

LuaEntity::LuaEntity(Entity entity, Layer *layer) : m_Entity(entity), m_Layer(layer) {}

LuaEntity::LuaEntity() {}

} // namespace Engine