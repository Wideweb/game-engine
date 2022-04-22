#pragma once

#include "Layer.hpp"
#include "LuaEntity.hpp"

#include <lua.hpp>

#include "LuaBridge/LuaBridge.h"

#include <glm/vec2.hpp>
#include <string>

namespace Engine {

class LuaLayer {
  private:
    Layer *m_Layer;

  public:
    LuaLayer(Layer *m_Layer);

    LuaEntity createEntity(std::string name);

    LuaEntity getEntity(std::string name);

    void setSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front,
                   std::string back);

    void load3D(std::string name, std::string obj, std::string diffuseMap, std::string specularMap,
                std::string normalMap);

    void load3D_v2(std::string name, std::string path);

    void loadTerrain(std::string name, std::string path, unsigned int width, unsigned int height, float maxHeight);

    void loadFont(std::string name, std::string path);

    void static add(lua_State *state);
};

} // namespace Engine