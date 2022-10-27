#pragma once

#include "Mesh.hpp"
#include "Model.hpp"
#include "ModelInstanceBatch.hpp"

#include <unordered_map>
#include <vector>

namespace Engine {

class Model3D : public Model {
  public:
    std::vector<Mesh> meshes;

    Model3D();
    Model3D(const std::vector<Mesh> &meshes);

    void setUp() override;
    void update() override;

    void setInstances(const std::vector<glm::mat4> &positions, const std::vector<uint32_t> &ids) const;

    void draw(ModelInstanceBatch* batch, Shader* activeShader = nullptr) override;

  private:
    unsigned int m_InstanceVBO, m_IdVBO;
};

} // namespace Engine