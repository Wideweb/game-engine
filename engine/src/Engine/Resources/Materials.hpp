#pragma once

#include "Material.hpp"
#include "Shader.hpp"
#include "Shaders.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

class Materials {
  private:
    std::unordered_map<std::string, std::shared_ptr<Material>> m_Map;

  public:
    std::shared_ptr<Material> defaultMaterial, meshMaterial, cubeEdgesMaterial, gridMaterial, terrainBrushMaterial, sceneToolMaterial;

    void init(const Shaders& shaders);
    void add(const std::string &name, Shader* shader);
    Material* get(const std::string &name);
};

} // namespace Engine
