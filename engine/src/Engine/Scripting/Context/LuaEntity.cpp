#include "LuaEntity.hpp"

#include "AIComponent.hpp"
#include "BehaviourComponent.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.hpp"
#include "DirectedLightComponent.hpp"
#include "Location2DComponent.hpp"
#include "LocationComponent.hpp"
#include "ParentComponent.hpp"
#include "ParticlesComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "SpotLightComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "StaticRender3DComponent.hpp"
#include "TagComponent.hpp"
#include "TerrainCollisionComponent.hpp"
#include "Text2DComponent.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

void LuaEntity::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginClass<LuaEntity>("Entity")
        .addFunction("addTagComponent", &LuaEntity::addComponent<TagComponent>)
        .addFunction("getTagComponent", &LuaEntity::getComponent<TagComponent>)
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
        .addFunction("hasStaticCollisionComponent", &LuaEntity::hasComponent<StaticCollisionComponent>)
        .addFunction("addDirectedLightComponent", &LuaEntity::addComponent<DirectedLightComponent>)
        .addFunction("addSpotLightComponent", &LuaEntity::addComponent<SpotLightComponent>)
        .addFunction("addPhysicsComponent", &LuaEntity::addComponent<PhysicsComponent>)
        .addFunction("addCameraComponent", &LuaEntity::addComponent<CameraComponent>)
        .addFunction("addBehaviourComponent", &LuaEntity::addComponent<BehaviourComponent>)
        .addFunction("getSkeletComponent", &LuaEntity::getComponent<SkeletComponent>)
        .addFunction("addSkeletComponent", &LuaEntity::addComponent<SkeletComponent>)
        .addFunction("addParticlesComponent", &LuaEntity::addComponent<ParticlesComponent>)
        .addFunction("addText2DComponent", &LuaEntity::addComponent<Text2DComponent>)
        .addFunction("getText2DComponent", &LuaEntity::getComponent<Text2DComponent>)
        .addFunction("addLocation2DComponent", &LuaEntity::addComponent<Location2DComponent>)
        .addFunction("getLocation2DComponent", &LuaEntity::getComponent<Location2DComponent>)
        .addFunction("addParentComponent", &LuaEntity::addComponent<ParentComponent>)
        .addFunction("getParentComponent", &LuaEntity::getComponent<ParentComponent>)
        .addFunction("getId", &LuaEntity::getId)
        .endClass();
}

LuaEntity::LuaEntity(Entity entity, Layer *layer) : m_Entity(entity), m_Layer(layer) {}

LuaEntity::LuaEntity() {}

} // namespace Engine