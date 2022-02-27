#include "glad/glad.h"
#include <iostream>

#include "Skybox.hpp"

namespace Engine {

Skybox::Skybox(std::vector<SkyboxVertex> vertices, Texture cubemapTexture)
    : vertices(std::move(vertices)), cubemapTexture(cubemapTexture) {
    this->setUp();
}

void Skybox::setUp() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(SkyboxVertex) * vertices.size()), vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Skybox::draw(Shader &shader) const {
    glActiveTexture(GL_TEXTURE0);
    cubemapTexture.bind();
    shader.setInt("skybox", 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    glBindVertexArray(0);
}

} // namespace Engine
