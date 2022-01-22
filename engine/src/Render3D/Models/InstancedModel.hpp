#pragma once

#include "Mesh.hpp"
#include "Model.hpp"
#include "ShaderModelInstanceManager.hpp"

#include <unordered_map>
#include <vector>

namespace Engine {

class InstancedModel : public Model {
  public:
    std::vector<Mesh> meshes;

    InstancedModel();
    InstancedModel(const std::vector<Mesh> &meshes);

    void setUp() override;
    void update() override;

    void setInstances(GLuint instanceVBO, GLuint idVBO, const std::vector<glm::mat4> &positions,
                      const std::vector<uint32_t> &ids) const;
    void updateInstances(GLuint instanceVBO, GLuint idVBO, size_t from, size_t to,
                         const std::vector<glm::mat4> &positions, const std::vector<uint32_t> &ids) const;

    void draw(Shader &shader, ShaderModelInstanceManager &instances, unsigned int textureShift) override;

  private:
    std::unordered_map<ShaderId, GLuint> shaderToInstanceVBO, shaderToIdVBO;
};

} // namespace Engine