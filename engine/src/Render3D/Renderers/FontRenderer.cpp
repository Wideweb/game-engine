#include "FontRenderer.hpp"

#include "File.hpp"
#include "Font.hpp"

#include "glad/glad.h"
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <string>

namespace Engine {

FontRenderer::FontRenderer(Viewport &viewport) : m_Viewport(viewport) {
    auto vertexSrc = File::readGLSL("./shaders/pass/font.vertex.glsl");
    auto fragmentSrc = File::readGLSL("./shaders/pass/font.fragment.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

FontRenderer::~FontRenderer() {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
}

void FontRenderer::draw(uint32_t id, std::string text, const Font &font, glm::mat4 transform, glm::mat4 ndcTransform,
                        glm::vec3 color) {
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(m_VAO);
    m_Shader.bind();

    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(m_Viewport.width), 0.0f, static_cast<float>(m_Viewport.height));

    m_Shader.setMatrix4("u_projection", projection);
    m_Shader.setMatrix4("u_model", transform);
    m_Shader.setMatrix4("u_ndcModel", ndcTransform);
    m_Shader.setFloat3("u_color", color);
    m_Shader.setInt("u_id", id);

    float x = 0;

    std::string::const_iterator i;
    for (i = text.begin(); i != text.end(); i++) {
        char code = *i;
        if (font.characters.find(code) == font.characters.end()) {
            code = '?';
        }

        const auto &character = font.characters.at(code);

        float posX = x + character.bearing.x;
        float posY = -(character.size.y - character.bearing.y);

        float w = character.size.x;
        float h = character.size.y;

        float vertices[6][4] = {{posX, posY + h, 0.0, 0.0}, {posX, posY, 0.0, 1.0},     {posX + w, posY, 1.0, 1.0},
                                {posX, posY + h, 0.0, 0.0}, {posX + w, posY, 1.0, 1.0}, {posX + w, posY + h, 1.0, 0.0}};

        m_Shader.setTexture("u_colorMap", character.texture);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += character.advance >> 6; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (!last_enable_blend) {
        glDisable(GL_BLEND);
    }

    if (last_enable_cull_face) {
        glEnable(GL_CULL_FACE);
    }

    if (last_enable_depth_test) {
        glEnable(GL_DEPTH_TEST);
    }
}

} // namespace Engine