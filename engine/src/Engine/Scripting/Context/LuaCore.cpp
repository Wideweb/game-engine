#include "LuaCore.hpp"

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
#include "MapUnitComponent.hpp"
#include "Material.hpp"

#include "Point.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Engine {

void LuaCore::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginClass<Material>("Material")
        .endClass();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<glm::vec2>("vec2")
        .addConstructor<void (*)(float, float)>()
        .addProperty("x", &glm::vec2::x)
        .addProperty("y", &glm::vec2::y)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<glm::vec3>("vec3")
        .addConstructor<void (*)(float, float, float)>()
        .addProperty("x", &glm::vec3::x)
        .addProperty("y", &glm::vec3::y)
        .addProperty("z", &glm::vec3::z)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<Point>("Point")
        .addConstructor<void (*)(uint32_t, uint32_t)>()
        .addProperty("x", &Point::x)
        .addProperty("y", &Point::y)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<TagComponent>("TagComponent")
        .addConstructor<void (*)(std::string)>()
        .addProperty("tag", &TagComponent::tag)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<LocationComponent>("LocationComponent")
        .addConstructor<void (*)(float, float, float)>()
        .addProperty("position", &LocationComponent::position)
        .addProperty("rotation", &LocationComponent::rotation)
        .addProperty("scale", &LocationComponent::scale)
        .addProperty("updated", &LocationComponent::updated)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<VelocityComponent>("VelocityComponent")
        .addConstructor<void (*)(void)>()
        .addProperty("velocity", &VelocityComponent::velocity)
        .addProperty("rotation", &VelocityComponent::rotation)
        .addProperty("speed", &VelocityComponent::speed)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<Render3DComponent>("Render3DComponent")
        .addConstructor<void (*)(std::string, Material*, float)>()
        .addProperty("rotation", &Render3DComponent::rotation)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<StaticRender3DComponent>("StaticRender3DComponent")
        .addConstructor<void (*)(std::string, float)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<SkeletState>("SkeletState")
        .addFunction("play", &SkeletState::play)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<SkeletComponent>("SkeletComponent")
        .addConstructor<void (*)(std::string)>()
        .addProperty("state", &SkeletComponent::state)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<CollisionComponent>("CollisionComponent")
        .addConstructor<void (*)(float, float, float)>()
        .addProperty("tag", &CollisionComponent::tag)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<TerrainCollisionComponent>("TerrainCollisionComponent")
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<StaticCollisionComponent>("StaticCollisionComponent")
        .addConstructor<void (*)(float, float, float)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<DirectedLight>("DirectedLight")
        .addConstructor<void (*)(void)>()
        .addProperty("ambient", &DirectedLight::ambient)
        .addProperty("diffuse", &DirectedLight::diffuse)
        .addProperty("specular", &DirectedLight::specular)
        .addProperty("nearPlane", &DirectedLight::shadowFrustumNearPlane)
        .addProperty("farPlane", &DirectedLight::shadowFrustumFarPlane)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<DirectedLightComponent>("DirectedLightComponent")
        .addConstructor<void (*)(DirectedLight)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<SpotLight>("SpotLight")
        .addConstructor<void (*)(void)>()
        .addProperty("ambient", &SpotLight::ambient)
        .addProperty("diffuse", &SpotLight::diffuse)
        .addProperty("specular", &SpotLight::specular)
        .addProperty("constant", &SpotLight::constant)
        .addProperty("linear", &SpotLight::linear)
        .addProperty("quadratic", &SpotLight::quadratic)
        .addProperty("nearPlane", &SpotLight::shadowFrustumNearPlane)
        .addProperty("farPlane", &SpotLight::shadowFrustumFarPlane)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<SpotLightComponent>("SpotLightComponent")
        .addConstructor<void (*)(SpotLight)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<PhysicsComponent>("PhysicsComponent")
        .addConstructor<void (*)(float)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<CameraComponent>("CameraComponent")
        .addConstructor<void (*)()>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<BehaviourComponent>("BehaviourComponent")
        .addConstructor<void (*)(std::string)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<ParticlesConfiguration>("ParticlesConfiguration")
        .addConstructor<void (*)(void)>()
        .addProperty("count", &ParticlesConfiguration::count)
        .addProperty("rate", &ParticlesConfiguration::rate)
        .addProperty("sizeFrom", &ParticlesConfiguration::sizeFrom)
        .addProperty("sizeTo", &ParticlesConfiguration::sizeTo)
        .addProperty("lifeTime", &ParticlesConfiguration::lifeTime)
        .addProperty("spawnRadius", &ParticlesConfiguration::spawnRadius)
        .addProperty("velocityMin", &ParticlesConfiguration::velocityMin)
        .addProperty("velocityMax", &ParticlesConfiguration::velocityMax)
        .addProperty("velocityPolarAngle", &ParticlesConfiguration::velocityPolarAngle)
        .addProperty("velocityAzimuthalAngle", &ParticlesConfiguration::velocityAzimuthalAngle)
        .addProperty("colorFrom", &ParticlesConfiguration::colorFrom)
        .addProperty("colorTo", &ParticlesConfiguration::colorTo)
        .addProperty("gravity", &ParticlesConfiguration::gravity)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<ParticlesComponent>("ParticlesComponent")
        .addConstructor<void (*)(ParticlesConfiguration)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<Text2DComponent>("Text2DComponent")
        .addConstructor<void (*)(std::string, std::string)>()
        .addProperty("color", &Text2DComponent::color)
        .addProperty("font", &Text2DComponent::font)
        .addProperty("text", &Text2DComponent::text)
        .addProperty("updated", &Text2DComponent::updated)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<Location2DComponent>("Location2DComponent")
        .addConstructor<void (*)(float, float)>()
        .addProperty("position", &Location2DComponent::position)
        .addProperty("rotation", &Location2DComponent::rotation)
        .addProperty("scale", &Location2DComponent::scale)
        .addProperty("updated", &Location2DComponent::updated)
        .addProperty("ndc", &Location2DComponent::ndc)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<ParentComponent>("ParentComponent")
        .addConstructor<void (*)(Entity)>()
        .addProperty("entity", &ParentComponent::entity)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<MapUnitComponent>("MapUnitComponent")
        .addConstructor<void (*)(uint32_t, uint32_t)>()
        .addProperty("target", &MapUnitComponent::target)
        .endClass()
        .endNamespace();
}

} // namespace Engine