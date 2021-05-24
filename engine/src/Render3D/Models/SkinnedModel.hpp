#pragma once

#include "Model.hpp"
#include "Skelet.hpp"
#include "SkinnedMesh.hpp"

#include <vector>

namespace Engine {

class SkinnedModel : public Model {
  public:
    std::vector<SkinnedMesh> meshes;
    Skelet skelet;

    SkinnedModel();
    SkinnedModel(const std::vector<SkinnedMesh> &meshes);

    void setUp() override;

    void draw(Shader &shader, ModelInstanceManager &instances,
              unsigned int textureShift) override;
};

} // namespace Engine