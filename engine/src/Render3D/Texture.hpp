#pragma once

#include "glad/glad.h"

#include <stddef.h>
#include <string>

namespace Engine {

class Texture {
  private:
    GLuint m_TextureID;
    uint32_t m_Index;

  public:
    Texture(const std::string &path);
    ~Texture();

    void bind();
    void unbind();

    uint32_t getId() { return m_TextureID; };
    uint32_t getIndex() { return m_Index; };
    void setIndex(uint32_t index) { m_Index = index; };

  private:
    bool load(const std::string &path);
};

} // namespace Engine
