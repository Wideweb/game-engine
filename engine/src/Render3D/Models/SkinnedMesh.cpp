#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

#include "SkinnedMesh.hpp"

namespace Engine {

SkinnedMesh::SkinnedMesh() {}
SkinnedMesh::~SkinnedMesh() {}

SkinnedMesh::SkinnedMesh(const std::vector<Vertex> &vertices, std::vector<GLuint> &indices)
    : vertices(vertices), indices(indices) {}

SkinnedMesh::SkinnedMesh(const SkinnedMesh &mesh) {
    VAO = mesh.VAO;
    EBO = mesh.EBO;
    VBO = mesh.VBO;
    instanceVBO = mesh.instanceVBO;

    vertices = mesh.vertices;
    indices = mesh.indices;
    material = mesh.material;
}

void SkinnedMesh::setUp() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * static_cast<GLuint>(indices.size()), indices.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // clang-format off
    GLsizei vec4Size = sizeof(glm::vec4);

    /////////////////////////////////////////////////////////////
    ////////////////////////// MODEL ////////////////////////////
    /////////////////////////////////////////////////////////////
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

    /////////////////////////////////////////////////////////////
    ///////////////////////// POSITION //////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(4);

    /////////////////////////////////////////////////////////////
    ////////////////////////// NORMAL ///////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(5);

    /////////////////////////////////////////////////////////////
    ////////////////////// TEXTURE COORD ////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(6);

    /////////////////////////////////////////////////////////////
    ///////////////////////// TANGENT ///////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(7);

    /////////////////////////////////////////////////////////////
    //////////////////////// BITANGENT //////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(11 * sizeof(float)));
    glEnableVertexAttribArray(8);

    /////////////////////////////////////////////////////////////
    ////////////////////////// COLOR ////////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(14 * sizeof(float)));
    glEnableVertexAttribArray(9);

    /////////////////////////////////////////////////////////////
    //////////////////////// JOINTS - 0 /////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribIPointer(10, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void *>(17 * sizeof(float)));
    glEnableVertexAttribArray(10);

    /////////////////////////////////////////////////////////////
    //////////////////////// JOINTS - 1 /////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribIPointer(11, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void *>(17 * sizeof(float) + 4 * sizeof(int)));
    glEnableVertexAttribArray(11);

    /////////////////////////////////////////////////////////////
    ////////////////////// JOINT WEIGHT - 0 /////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(17 * sizeof(float) + 8 * sizeof(int)));
    glEnableVertexAttribArray(12);

    /////////////////////////////////////////////////////////////
    ////////////////////// JOINT WEIGHT - 1 /////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(21 * sizeof(float) + 8 * sizeof(int)));
    glEnableVertexAttribArray(13);

    /////////////////////////////////////////////////////////////
    ////////////////////// JOINTS NUMBER /////////.//////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribIPointer(14, 1, GL_INT, sizeof(Vertex), reinterpret_cast<void *>(25 * sizeof(float) + 8 * sizeof(int)));
    glEnableVertexAttribArray(14);
    // clang-format on

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void SkinnedMesh::draw(Shader &shader, const glm::mat4 &position, const std::vector<glm::mat4> &joints,
                       unsigned int textureShift) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::mat4)), glm::value_ptr(position),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (hasMaterial) {
        shader.setInt("u_hasMaterial", 1);

        glActiveTexture(GL_TEXTURE0 + textureShift);
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

    shader.setInt("u_jointsNumber", static_cast<int>(joints.size()));
    for (size_t i = 0; i < joints.size(); i++) {
        shader.setMatrix4("u_joints[" + std::to_string(i) + "]", joints[i]);
    }

    glBindVertexArray(VAO);

    if (indices.size() > 0) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    }

    glBindVertexArray(0);
}

} // namespace Engine
