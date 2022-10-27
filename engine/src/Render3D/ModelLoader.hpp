#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Model.hpp"
#include "Skybox.hpp"

namespace Engine {

class ModelLoader {
  public:
    static std::shared_ptr<Model> load(const std::string &path);

    static std::shared_ptr<Model> loadObj(const std::string &path);

    static std::shared_ptr<Skybox> loadSkybox(const std::vector<std::string> &faces);

    static std::shared_ptr<Model> loadTerrain(const std::string &path, unsigned int terrainWidth,
                                              unsigned int terrainHeight, float maxHeight);
};

} // namespace Engine
