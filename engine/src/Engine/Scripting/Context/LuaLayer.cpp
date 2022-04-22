#include "LuaLayer.hpp"

#include "Application.hpp"
#include "LuaEntity.hpp"
#include "ModelLoader.hpp"

namespace Engine {

void LuaLayer::add(lua_State *state) {
    luabridge::getGlobalNamespace(state)
        .beginClass<LuaLayer>("Layer")
        .addFunction("load3D", &LuaLayer::load3D)
        .addFunction("load3D_v2", &LuaLayer::load3D_v2)
        .addFunction("loadTerrain", &LuaLayer::loadTerrain)
        .addFunction("loadFont", &LuaLayer::loadFont)
        .addFunction("setSkybox", &LuaLayer::setSkybox)
        .addFunction("createEntity", &LuaLayer::createEntity)
        .addFunction("getEntity", &LuaLayer::getEntity)
        .endClass();
}

LuaLayer::LuaLayer(Layer *layer) : m_Layer(layer) {}

LuaEntity LuaLayer::createEntity(std::string name) {
    return LuaEntity{m_Layer->getCoordinator().CreateEntity(name), m_Layer};
}

LuaEntity LuaLayer::getEntity(std::string name) {
    return LuaEntity{m_Layer->getCoordinator().GetEntity(name), m_Layer};
}

void LuaLayer::setSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front,
                         std::string back) {
    auto skybox_model = ModelLoader::loadSkybox({right, left, top, bottom, front, back});
    m_Layer->getScene().setSkybox(skybox_model);
}

void LuaLayer::load3D(std::string name, std::string obj, std::string diffuseMap, std::string specularMap,
                      std::string normalMap) {
    auto model = ModelLoader::load(obj, diffuseMap, specularMap, normalMap);
    Application::get().getModels().RegisterModel(name, model);
}

void LuaLayer::load3D_v2(std::string name, std::string path) {
    auto model = ModelLoader::load(path);
    Application::get().getModels().RegisterModel(name, model);
}

void LuaLayer::loadTerrain(std::string name, std::string path, unsigned int width, unsigned int height,
                           float maxHeight) {
    auto model = ModelLoader::loadTerrain(path, width, height, maxHeight);
    Application::get().getModels().RegisterModel(name, model);
}

void LuaLayer::loadFont(std::string name, std::string path) { Application::get().getFonts().Add(name, path); }

} // namespace Engine