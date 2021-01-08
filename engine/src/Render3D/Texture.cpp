#include "Texture.hpp"

namespace Engine {

static GLenum getGLTextureType(Texture::TextureType type) {
    switch (type) {
    case Texture::TextureType::SIMPLE:
        return GL_TEXTURE_2D;
    case Texture::TextureType::CUBE_MAP:
        return GL_TEXTURE_CUBE_MAP;
    default:
        return GL_TEXTURE_2D;
    }
}

Texture::Texture(GLuint id, TextureType type) : m_TextureID(id), m_Type(type) {}

void Texture::bind() { glBindTexture(getGLTextureType(m_Type), m_TextureID); }

void Texture::unbind() { glBindTexture(getGLTextureType(m_Type), 0); }

Texture::~Texture() { glDeleteTextures(1, &m_TextureID); }

} // namespace Engine
