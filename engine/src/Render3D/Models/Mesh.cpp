#include "glad/glad.h"
#include <iostream>

#include "Mesh.hpp"

namespace Engine {

Mesh::Mesh() {}
Mesh::~Mesh() {}

Mesh::Mesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices) : vertices(vertices), indices(indices) {}

Mesh::Mesh(const std::vector<Vertex> &vertices) : vertices(vertices) {}

Mesh::Mesh(const Mesh &mesh) {
    VAO = mesh.VAO;
    EBO = mesh.EBO;
    VBO = mesh.VBO;

    vertices = mesh.vertices;
    indices = mesh.indices;
    // material = mesh.material;
}

void Mesh::setUp() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * static_cast<GLuint>(indices.size()), indices.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(),
                 GL_STATIC_DRAW);

    /////////////////////////////////////////////////////////////
    ///////////////////////// POSITION //////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(5);

    /////////////////////////////////////////////////////////////
    ////////////////////////// NORMAL ///////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(6);

    /////////////////////////////////////////////////////////////
    ////////////////////// TEXTURE COORD ////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(7);

    /////////////////////////////////////////////////////////////
    ///////////////////////// TANGENT ///////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(8);

    /////////////////////////////////////////////////////////////
    //////////////////////// BITANGENT //////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(11 * sizeof(float)));
    glEnableVertexAttribArray(9);

    /////////////////////////////////////////////////////////////
    ////////////////////////// COLOR ////////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(14 * sizeof(float)));
    glEnableVertexAttribArray(10);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::update() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * static_cast<GLuint>(indices.size()), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setInstances(GLuint idVBO, GLuint instanceVBO) const {
    // 4.3
    // glVertexArrayVertexBuffer(VAO, 1, idVBO, reinterpret_cast<void *>(0), sizeof(uint32_t));
    // glVertexArrayVertexBuffer(VAO, 2, instanceVBO, reinterpret_cast<void *>(0), sizeof(glm::vec4) * 4);

    glBindVertexArray(VAO);

    GLsizei vec4Size = sizeof(glm::vec4);

    /////////////////////////////////////////////////////////////
    //////////////////////////// ID /////////////////////////////
    /////////////////////////////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);

    glVertexAttribIPointer(0, 1, GL_INT, sizeof(uint32_t), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribDivisor(0, 1);

    /////////////////////////////////////////////////////////////
    ////////////////////////// MODEL ////////////////////////////
    /////////////////////////////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(vec4Size));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(2 * vec4Size));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void *>(3 * vec4Size));
    glEnableVertexAttribArray(4);

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::draw(size_t instanceCount) const {
    // shader.bind();

    // if (!material.empty()) {
    //     shader.setInt("u_hasMaterial", 1);

    //     if (!material.diffuseMap.empty()) {
    //         shader.setInt("u_material.hasDiffuse", 1);
    //         shader.setTexture("u_material.diffuse", material.diffuseMap);
    //     } else {
    //         shader.setInt("u_material.hasDiffuse", 0);
    //     }

    //     if (!material.specularMap.empty()) {
    //         shader.setInt("u_material.hasSpecular", 1);
    //         shader.setTexture("u_material.specular", material.specularMap);
    //     } else {
    //         shader.setInt("u_material.hasSpecular", 0);
    //     }

    //     if (!material.normalMap.empty()) {
    //         shader.setInt("u_material.hasNormal", 1);
    //         shader.setTexture("u_material.normal", material.normalMap);
    //     } else {
    //         shader.setInt("u_material.hasNormal", 0);
    //     }

    //     if (!material.metallicMap.empty()) {
    //         shader.setInt("u_material.hasMetallic", 1);
    //         shader.setTexture("u_material.metallic", material.metallicMap);
    //     } else {
    //         shader.setInt("u_material.hasMetallic", 0);
    //     }

    //     if (!material.roughnessMap.empty()) {
    //         shader.setInt("u_material.hasRoughness", 1);
    //         shader.setTexture("u_material.roughness", material.roughnessMap);
    //     } else {
    //         shader.setInt("u_material.hasRoughness", 0);
    //     }

    //     if (!material.ambientOcclusionMap.empty()) {
    //         shader.setInt("u_material.hasAmbientOcclusion", 1);
    //         shader.setTexture("u_material.ambientOcclusion", material.ambientOcclusionMap);
    //     } else {
    //         shader.setInt("u_material.hasAmbientOcclusion", 0);
    //     }

    //     shader.setFloat("u_material.shininess", material.shininess);
    // } else {
    //     shader.setInt("u_hasMaterial", 0);
    // }

    glBindVertexArray(VAO);

    if (indices.size() > 0) {
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0,
                                static_cast<GLsizei>(instanceCount));
    } else {
        glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()),
                              static_cast<GLsizei>(instanceCount));
    }

    glBindVertexArray(0);
}

} // namespace Engine
