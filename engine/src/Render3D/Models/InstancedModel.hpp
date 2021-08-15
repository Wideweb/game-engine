#pragma once

#include "InstancedMesh.hpp"
#include "Model.hpp"

#include <vector>

namespace Engine {

class InstancedModel : public Model {
  public:
    std::vector<InstancedMesh> meshes;

    InstancedModel();
    InstancedModel(const std::vector<InstancedMesh> &meshes);

    void setUp() override;
    void update() override;

    void draw(Shader &shader, ModelInstanceManager &instances, unsigned int textureShift) override;
};

} // namespace Engine