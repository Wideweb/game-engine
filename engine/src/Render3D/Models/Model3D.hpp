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

    InstancedModel();
    InstancedModel(const std::vector<Mesh> &meshes);

    void setUp() override;
    void update() override;

    void setInstances(unsigned int instanceVBO, unsigned int idVBO, const std::vector<glm::mat4> &positions,
                      const std::vector<uint32_t> &ids) const;
    void updateInstances(unsigned int instanceVBO, unsigned int idVBO, size_t from, size_t to,
                         const std::vector<glm::mat4> &positions, const std::vector<uint32_t> &ids) const;

    void draw(ModelInstanceBatch* batch, Shader* activeShader = nullptr) override;

  private:
    unsigned int m_InstanceVBO, m_IdVBO;
};

} // namespace Engine