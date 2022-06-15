#include "Renderer2D.hpp"

#include "File.hpp"
#include "glad/glad.h"

#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

Renderer2D::Renderer2D(Viewport &viewport) : m_Viewport(viewport) {
    auto vertexSrc = File::read("./shaders/screen-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/screen-fragment-shader.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    /////////////////////////////////////////////////////////////
    ///////////////////////// POSITION //////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh2D::Vertex), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    /////////////////////////////////////////////////////////////
    ////////////////////// TEXTURE COORD ////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh2D::Vertex),
                          reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /////////////////////////////////////////////////////////////
    ////////////////////////// COLOR ////////////////////////////
    /////////////////////////////////////////////////////////////
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Mesh2D::Vertex),
                          reinterpret_cast<void *>(4 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer2D::~Renderer2D() {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
    unsigned int buffers[2]{m_VBO, m_EBO};
    glDeleteBuffers(2, buffers);
    m_VBO = 0;
    m_EBO = 0;
    m_Shader.free();
}

void Renderer2D::draw(Texture &texture, glm::vec2 position, glm::vec2 size, glm::vec4 color, uint32_t id) {
    std::vector<Mesh2D::Vertex> vertices;
    vertices.push_back(Mesh2D::Vertex(glm::vec2(-.5f, -.5f), glm::vec2(0.0f, 1.0f), color));
    vertices.push_back(Mesh2D::Vertex(glm::vec2(-.5f, 0.5f), glm::vec2(0.0f, 0.0f), color));
    vertices.push_back(Mesh2D::Vertex(glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.0f), color));
    vertices.push_back(Mesh2D::Vertex(glm::vec2(0.5f, -.5f), glm::vec2(1.0f, 1.0f), color));

    std::vector<uint32_t> indices{0, 2, 1, 0, 3, 2};

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    draw(vertices, indices, texture, model, id);
}

void Renderer2D::draw(const std::vector<Mesh2D::Vertex> &vertices, const std::vector<uint32_t> &indices,
                      Texture &texture, const glm::mat4 &model, uint32_t id) {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Mesh2D::Vertex) * vertices.size()), vertices.data(),
                 GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * static_cast<GLuint>(indices.size()), indices.data(),
                 GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(m_Viewport.width), 0.0f, static_cast<float>(m_Viewport.height));

    m_Shader.bind();
    m_Shader.setInt("u_id", id);
    m_Shader.setMatrix4("u_projection", projection);
    m_Shader.setMatrix4("u_model", model);
    m_Shader.setTexture("u_colorMap", texture);

    GLint last_scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb;
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint *)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb;
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint *)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint *)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint *)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint *)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint *)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
    // GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    // glEnable(GL_SCISSOR_TEST);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);

    if (!last_enable_blend) {
        glDisable(GL_BLEND);
    }

    if (last_enable_cull_face) {
        glEnable(GL_CULL_FACE);
    }

    if (last_enable_depth_test) {
        glEnable(GL_DEPTH_TEST);
    }

    if (last_enable_stencil_test) {
        glEnable(GL_STENCIL_TEST);
    }

    // if (!last_enable_scissor_test) {
    //     glDisable(GL_SCISSOR_TEST);
    // }
}

} // namespace Engine