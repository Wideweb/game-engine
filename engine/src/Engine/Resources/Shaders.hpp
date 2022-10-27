#pragma once

#include "Shader.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

class Shaders {
  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Map;

  public:
    std::shared_ptr<Shader> defaultShader, phongShader, pbrShader, meshShader, cubeEdgesShader, gridShader, terrainBrushShader;

    void init();
    void load(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
    Shader* get(const std::string &name);
};

} // namespace Engine
