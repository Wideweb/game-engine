#include "Model3D.hpp"

#include "glad/glad.h"

namespace Engine {

Model3D::Model3D() {}

Model3D::Model3D(const std::vector<Mesh> &meshes) : meshes(meshes) {}

void Model3D::setUp() {
    glGenBuffers(1, &m_IdVBO);
    glGenBuffers(1, &m_InstanceVBO);

    for (auto &mesh : meshes) {
        mesh.setUp();
        mesh.setInstances(m_IdVBO, m_InstanceVBO);
    }
}

void Model3D::update() {
    for (auto &mesh : meshes) {
        mesh.update();
    }
}

void Model3D::draw(ModelInstanceBatch* batch, Shader* activeShader) {
    setInstances(batch.positions(), instances.ids());

    for (const auto &mesh : meshes) {
        mesh.draw(batch.size());
    }
}

void Model3D::setInstances(const std::vector<glm::mat4> &positions, const std::vector<uint32_t> &ids) const {
    auto size = static_cast<GLint>(sizeof(glm::mat4) * positions.size());
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, size, positions.data(), GL_STATIC_DRAW);

    size = static_cast<GLint>(sizeof(uint32_t) * ids.size());
    glBindBuffer(GL_ARRAY_BUFFER, m_IdVBO);
    glBufferData(GL_ARRAY_BUFFER, size, ids.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace Engine
