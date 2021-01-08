#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

namespace Engine {

struct SkyboxVertex {
    glm::vec3 position;

    SkyboxVertex(glm::vec3 position) : position(std::move(position)) {}
};

class Skybox {
  public:
    std::vector<SkyboxVertex> vertices;
    std::shared_ptr<Texture> cubemapTexture;

    Skybox(std::vector<SkyboxVertex> vertices,
           std::shared_ptr<Texture> cubemapTexture);

    void draw(Shader &shader) const;

  private:
    GLuint VAO, VBO;

    void setUp();
};

} // namespace Engine
