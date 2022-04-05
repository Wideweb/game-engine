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
    CubeMapTexture cubemapTexture;
    float rotation, rotationSpeed = 0.0f;

    Skybox(std::vector<SkyboxVertex> vertices, CubeMapTexture cubemapTexture);
    ~Skybox();

    void draw(Shader &shader) const;

  private:
    unsigned int VAO, VBO;

    void setUp();
};

} // namespace Engine
