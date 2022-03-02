#pragma once

#include "Model.hpp"
#include "ShaderModelInstanceManager.hpp"
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

    void draw(Shader &shader, ShaderModelInstanceManager &instances) override;
};

} // namespace Engine