#include "glad/glad.h"
#include <iostream>

#include "Mesh.hpp"

namespace Engine {

Mesh::Mesh() {}
Mesh::~Mesh() {}

Mesh::Mesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
           const Material &material)
    : vertices(vertices), indices(indices), material(material) {
    this->setUp();
}

Mesh::Mesh(const Mesh &mesh) {
    VAO = mesh.VAO;
    EBO = mesh.EBO;
    VBO = mesh.VBO;

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

    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
    //              indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) const {
    glActiveTexture(GL_TEXTURE0);
    // std::cout << glGetError() << std::endl;
    material.diffuseMap->bind();
    shader.setInt("Material.diffuse", 0);

    glActiveTexture(GL_TEXTURE1);
    // std::cout << glGetError() << std::endl;
    material.specularMap->bind();
    shader.setInt("Material.specular", 1);

    glActiveTexture(GL_TEXTURE2);
    // std::cout << glGetError() << std::endl;
    material.normalMap->bind();
    shader.setInt("Material.normal", 2);

    shader.setFloat("Material.shininess", material.shininess);

    glBindVertexArray(VAO);
    // std::cout << glGetError() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // std::cout << glGetError() << std::endl;
    glBindVertexArray(0);
}

} // namespace Engine
