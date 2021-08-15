#include "glad/glad.h"
#include <iostream>

#include "InstancedMesh.hpp"

namespace Engine {

InstancedMesh::InstancedMesh() {}
InstancedMesh::~InstancedMesh() {}

InstancedMesh::InstancedMesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
                             const Material &material)
    : vertices(vertices), indices(indices), material(material), hasMaterial(true) {}

InstancedMesh::InstancedMesh(const InstancedMesh &mesh) {
    VAO = mesh.VAO;
    EBO = mesh.EBO;
    VBO = mesh.VBO;
    instanceVBO = mesh.instanceVBO;

    vertices = mesh.vertices;
    indices = mesh.indices;
    material = mesh.material;
    hasMaterial = mesh.hasMaterial;
}

void InstancedMesh::setUp() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * static_cast<GLuint>(indices.size()), indices.data(),
                 GL_STATIC_DRAW);

    // clang-format off
    GLsizei vec4Size = sizeof(glm::vec4);

    /////////////////////////////////////////////////////////////
    //////////////////////////// ID /////////////////////////////
    /////////////////////////////////////////////////////////////
    glGenBuffers(1, &idVBO);
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(uint32_t), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0); 

    glVertexAttribDivisor(0, 1);

    /////////////////////////////////////////////////////////////
    ////////////////////////// MODEL ////////////////////////////
    /////////////////////////////////////////////////////////////
    glGenBuffers(1, &instanceVBO);
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

    /////////////////////////////////////////////////////////////
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()),
                 vertices.data(), GL_STATIC_DRAW);

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
    // clang-format on

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void InstancedMesh::update() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::setInstances(const std::vector<glm::mat4> &positions, const std::vector<uint32_t> &ids) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::mat4) * positions.size()), positions.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * ids.size()), ids.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::updateInstances(size_t from, size_t to, const std::vector<glm::mat4> &positions,
                                    const std::vector<uint32_t> &ids) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::mat4) * from),
                    static_cast<GLsizeiptr>(sizeof(glm::mat4) * (to - from + 1)), positions.data() + from);

    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * from),
                    static_cast<GLsizeiptr>(sizeof(uint32_t) * (to - from + 1)), ids.data() + from);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::draw(Shader &shader, size_t instanceCount, unsigned int textureShift = 0) const {
    glActiveTexture(GL_TEXTURE0 + textureShift);

    if (hasMaterial) {
        shader.setInt("u_hasMaterial", 1);
        material.diffuseMap->bind();
        // shader.setInt("u_material.diffuse", static_cast<int>(textureShift));

        glActiveTexture(GL_TEXTURE1 + textureShift);
        material.specularMap->bind();
        // shader.setInt("u_material.specular", static_cast<int>(textureShift) + 1);

        glActiveTexture(GL_TEXTURE2 + textureShift);
        material.normalMap->bind();
        // shader.setInt("u_material.normal", static_cast<int>(textureShift) + 2);

        shader.setFloat("u_material.shininess", material.shininess);
    } else {
        shader.setInt("u_hasMaterial", 0);
    }

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
