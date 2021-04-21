#include "glad/glad.h"
#include <iostream>

#include "InstancedMesh.hpp"

namespace Engine {

InstancedMesh::InstancedMesh() {}
InstancedMesh::~InstancedMesh() {}

InstancedMesh::InstancedMesh(const std::vector<Vertex> &vertices,
                             std::vector<GLuint> &indices,
                             const Material &material)
    : vertices(vertices), indices(indices), material(material) {}

InstancedMesh::InstancedMesh(const InstancedMesh &mesh) {
    VAO = mesh.VAO;
    EBO = mesh.EBO;
    VBO = mesh.VBO;
    instanceVBO = mesh.instanceVBO;

    vertices = mesh.vertices;
    indices = mesh.indices;
    material = mesh.material;
}

void InstancedMesh::setUp() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * static_cast<GLuint>(indices.size()),
                 indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // clang-format off
    GLsizei vec4Size = sizeof(glm::vec4);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0); 

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(vec4Size));
    glEnableVertexAttribArray(1); 

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(2 * vec4Size));
    glEnableVertexAttribArray(2); 

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(3 * vec4Size));
    glEnableVertexAttribArray(3);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()),
                 vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(6);
    // clang-format on

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void InstancedMesh::setInstances(
    const std::vector<glm::mat4> &positions) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sizeof(glm::mat4) * positions.size()),
                 positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::updateInstances(
    size_t from, size_t to, const std::vector<glm::mat4> &positions) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(
        GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::mat4) * from),
        static_cast<GLsizeiptr>(sizeof(glm::mat4) * (to - from + 1)),
        positions.data() + from);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::draw(Shader &shader, size_t instanceCount,
                         unsigned int textureShift = 0) const {
    glActiveTexture(GL_TEXTURE0 + textureShift);
    material.diffuseMap->bind();
    shader.setInt("Material.diffuse", static_cast<int>(textureShift));

    glActiveTexture(GL_TEXTURE1 + textureShift);
    material.specularMap->bind();
    shader.setInt("Material.specular", static_cast<int>(textureShift) + 1);

    glActiveTexture(GL_TEXTURE2 + textureShift);
    material.normalMap->bind();
    shader.setInt("Material.normal", static_cast<int>(textureShift) + 2);

    shader.setFloat("Material.shininess", material.shininess);

    glBindVertexArray(VAO);

    if (indices.size() > 0) {
        glDrawElementsInstanced(
            GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT,
            0, static_cast<GLsizei>(instanceCount));
    } else {
        glDrawArraysInstanced(GL_TRIANGLES, 0,
                              static_cast<GLsizei>(vertices.size()),
                              static_cast<GLsizei>(instanceCount));
    }

    glBindVertexArray(0);
}

} // namespace Engine
