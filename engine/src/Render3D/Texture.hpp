#pragma once

#include "glad/glad.h"

#include <stddef.h>
#include <string>

namespace Engine {

class Texture {
  public:
    enum class TextureType { SIMPLE, CUBE_MAP };

    Texture(GLuint id, TextureType type = TextureType::SIMPLE);
    ~Texture();

    void bind();
    void unbind();

    uint32_t getId() { return m_TextureID; }

  private:
    GLuint m_TextureID;
    TextureType m_Type;
};

} // namespace Engine
