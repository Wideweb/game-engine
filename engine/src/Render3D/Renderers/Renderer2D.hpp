#pragma once

#include "Mesh2D.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Engine {

class Renderer2D {
  private:
    Shader m_Shader;
    unsigned int m_VAO, m_VBO, m_EBO;

  public:
    Renderer2D();
    ~Renderer2D();

    void draw(const std::vector<Mesh2D::Vertex> &vertices, const std::vector<uint32_t> &indices, const Texture *texture,
              const glm::mat4 &model);
};

} // namespace Engine