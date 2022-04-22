#pragma once

#include "Font.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

namespace Engine {

class FontRenderer {
  private:
    Shader m_Shader;
    unsigned int m_VAO, m_VBO;

    Viewport &m_Viewport;

  public:
    FontRenderer(Viewport &viewport);
    ~FontRenderer();

    void draw(uint32_t id, std::string text, const Font &font, glm::mat4 transform, glm::mat4 ndcTransform, glm::vec3 color);
};

} // namespace Engine