#include "glad/glad.h"
#include <iostream>

#include "Mesh.hpp"

namespace Engine {

Mesh::Mesh() {}
Mesh::~Mesh() {}

Mesh::Mesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
           const Material &material)
    : vertices(vertices), indices(indices), material(material) {}

Mesh::Mesh(const Mesh &mesh) {
    VAO = mesh.VAO;
    EBO = mesh.EBO;
    VBO = mesh.VBO;
    instanceVBO = mesh.instanceVBO;

    vertices = mesh.vertices;
    indices = mesh.indices;
    material = mesh.material;
}

void Mesh::setUp() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()),
                 vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * static_cast<GLuint>(indices.size()),
                 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex),
                           reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(
        4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void *>(8 * sizeof(float) + 4 * sizeof(int)));
    glEnableVertexAttribArray(4);

    glVertexAttribIPointer(
        5, 1, GL_INT, sizeof(Vertex),
        reinterpret_cast<void *>(12 * sizeof(float) + 4 * sizeof(int)));
    glEnableVertexAttribArray(5);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // clang-format off
    GLsizei vec4Size = sizeof(glm::vec4);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(6); 

    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(vec4Size));
    glEnableVertexAttribArray(7); 

    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(2 * vec4Size));
    glEnableVertexAttribArray(8); 

    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(3 * vec4Size));
    glEnableVertexAttribArray(9);
    // clang-format on

    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);
    glVertexAttribDivisor(9, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::setInstances(const std::vector<glm::mat4> &positions) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sizeof(glm::mat4) * positions.size()),
                 positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::updateInstances(size_t from, size_t to,
                           const std::vector<glm::mat4> &positions) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(
        GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::mat4) * from),
        static_cast<GLsizeiptr>(sizeof(glm::mat4) * (to - from + 1)),
        positions.data() + from);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader &shader, size_t instanceCount,
                unsigned int textureShift = 0) const {
    glActiveTexture(GL_TEXTURE0 + textureShift);
    // std::cout << glGetError() << std::endl;
    material.diffuseMap->bind();
    shader.setInt("Material.diffuse", static_cast<int>(textureShift));

    glActiveTexture(GL_TEXTURE1 + textureShift);
    // std::cout << glGetError() << std::endl;
    material.specularMap->bind();
    shader.setInt("Material.specular", static_cast<int>(textureShift) + 1);

    glActiveTexture(GL_TEXTURE2 + textureShift);
    // std::cout << glGetError() << std::endl;
    material.normalMap->bind();
    shader.setInt("Material.normal", static_cast<int>(textureShift) + 2);

    shader.setFloat("Material.shininess", material.shininess);

    glBindVertexArray(VAO);
    // std::cout << glGetError() << std::endl;

    if (indices.size() > 0) {
        glDrawElementsInstanced(
            GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT,
            0, static_cast<GLsizei>(instanceCount));
    } else {
        glDrawArraysInstanced(GL_TRIANGLES, 0,
                              static_cast<GLsizei>(vertices.size()),
                              static_cast<GLsizei>(instanceCount));
    }

    // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // std::cout << glGetError() << std::endl;
    glBindVertexArray(0);
}

} // namespace Engine
