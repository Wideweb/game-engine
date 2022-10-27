#pragma once

#include "Model.hpp"
#include "ModelInstanceBatch.hpp"
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
    void update() override;

    void draw(ModelInstanceBatch *batch, Shader* activeShader = nullptr) override;
};

} // namespace Engine