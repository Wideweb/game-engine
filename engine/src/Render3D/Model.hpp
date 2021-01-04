#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Mesh.hpp"

namespace Engine {

class Model {
  public:
    std::vector<Mesh> meshes;

    Model(const std::vector<Mesh> &meshes);
    ~Model();
};

} // namespace Engine