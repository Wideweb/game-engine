#include "LuaCore.hpp"

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

#include <glm/vec3.hpp>

namespace Engine {

void LuaCore::add(lua_State *state) {
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
        .beginClass<LocationComponent>("LocationComponent")
        .addConstructor<void (*)(float, float, float)>()
        .addProperty("position", &LocationComponent::position)
        .addProperty("rotation", &LocationComponent::rotation)
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
        .addConstructor<void (*)(std::string, float)>()
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
        .beginClass<SkeletComponent>("SkeletComponent")
        .addConstructor<void (*)(std::string)>()
        .addProperty("animation", &SkeletComponent::animation)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<CollisionComponent>("CollisionComponent")
        .addConstructor<void (*)(float, float, float)>()
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
        .beginClass<Light>("Light")
        .addConstructor<void (*)(void)>()
        .addProperty("ambient", &Light::ambient)
        .addProperty("diffuse", &Light::diffuse)
        .addProperty("specular", &Light::specular)
        .addProperty("constant", &Light::constant)
        .addProperty("linear", &Light::linear)
        .addProperty("quadratic", &Light::quadratic)
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<Light3DComponent>("Light3DComponent")
        .addConstructor<void (*)(Light)>()
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
        .addConstructor<void (*)(float, float, float)>()
        .endClass()
        .endNamespace();

    luabridge::getGlobalNamespace(state)
        .beginNamespace("Core")
        .beginClass<BehaviourComponent>("BehaviourComponent")
        .addConstructor<void (*)(std::string)>()
        .endClass()
        .endNamespace();
}

} // namespace Engine