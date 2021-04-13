#pragma once

#include "Layer.hpp"
#include "LuaEntity.hpp"

#include "lua.hpp"
#include <LuaBridge/LuaBridge.h>

#include <string>

namespace Engine {

class LuaLayer {
  private:
    Layer *m_Layer;

  public:
    LuaLayer(Layer *m_Layer);

    LuaEntity createEntity(std::string name);

    LuaEntity getEntity(std::string name);

    void setSkybox(std::string right, std::string left, std::string top,
                   std::string bottom, std::string front, std::string back);

    void loadModel3D(std::string name, std::string obj, std::string diffuseMap,
                     std::string specularMap, std::string normalMap);

    void static add(lua_State *state);
};

} // namespace Engine