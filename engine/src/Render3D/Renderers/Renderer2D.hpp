#pragma once

#include "Mesh2D.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

namespace Engine {

class Renderer2D {
  private:
    Shader m_Shader;
    unsigned int m_VAO, m_VBO, m_EBO;
    Viewport &m_Viewport;

  public:
    Renderer2D(Viewport &viewport);
    ~Renderer2D();

    void draw(const std::vector<Mesh2D::Vertex> &vertices, const std::vector<uint32_t> &indices, Texture &texture,
              const glm::mat4 &model, uint32_t id = 0);
    void draw(Texture &texture, glm::vec2 position, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f), uint32_t id = 0);
};

} // namespace Engine