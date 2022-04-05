#include "Texture.hpp"

#include "glad/glad.h"

#include <stdexcept>

namespace Engine {

static GLenum getGLTextureType(Texture::TextureType type) {
    switch (type) {
    case Texture::TextureType::COLOR:
        return GL_TEXTURE_2D;
    case Texture::TextureType::CUBE_MAP:
        return GL_TEXTURE_CUBE_MAP;
    case Texture::TextureType::DEPTH_BUFFER:
        return GL_TEXTURE_2D;
    default:
        return GL_TEXTURE_2D;
    }
}

void Texture::bind() const { glBindTexture(getGLTextureType(type), id); }

void Texture::unbind() const { glBindTexture(getGLTextureType(type), 0); }

void Texture::resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    glTexImage2D(getGLTextureType(type), 0, GfxImage::getNativeFormat(format), width, height, 0,
                 GfxImage::getNativeDataFormat(dataFormat), GfxImage::getNativeDataType(dataType), NULL);
}

void Texture::free() {
    if (!empty()) {
        glDeleteTextures(1, &id);
        setEmpty();
    }
}

Texture Texture::createDepthBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::DEPTH_BUFFER;
    texture.format = Texture::InternalFormat::DEPTH_COMPONENT;
    texture.dataFormat = Texture::DataFormat::DEPTH_COMPONENT;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

CubeMapTexture CubeMapTexture::createCubeDepthBuffer(int width, int height) {
    CubeMapTexture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
                     GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::CUBE_MAP;
    texture.format = Texture::InternalFormat::DEPTH_COMPONENT;
    texture.dataFormat = Texture::DataFormat::DEPTH_COMPONENT;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

CubeMapTexture CubeMapTexture::createCubeMap() {
    CubeMapTexture texture;
    texture.width = 0;
    texture.height = 0;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::CUBE_MAP;
    texture.dataType = Texture::DataType::UNSIGNED_BYTE;

    return texture;
}

Texture Texture::createRGBA8FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGBA8F;
    texture.dataFormat = Texture::DataFormat::RGBA;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createRGBA16FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGBA16F;
    texture.dataFormat = Texture::DataFormat::RGBA;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createRGBA32FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGBA32F;
    texture.dataFormat = Texture::DataFormat::RGBA;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createRGB8FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGB8F;
    texture.dataFormat = Texture::DataFormat::RGB;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createRGB16FBuffer(int width, int height, void *data) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGB16F;
    texture.dataFormat = Texture::DataFormat::RGB;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createRGB32FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGB32F;
    texture.dataFormat = Texture::DataFormat::RGB;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createRGB8IBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8I, width, height, 0, GL_RGB, GL_INT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGB8I;
    texture.dataFormat = Texture::DataFormat::RGB;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createR32IBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_INT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::R32I;
    texture.dataFormat = Texture::DataFormat::RED_INTEGER;
    texture.dataType = Texture::DataType::INT;

    return texture;
}

Texture Texture::createR8Buffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::R8F;
    texture.dataFormat = Texture::DataFormat::RED;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createR16FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::R16F;
    texture.dataFormat = Texture::DataFormat::RED;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

Texture Texture::createR32FBuffer(int width, int height) {
    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::R32F;
    texture.dataFormat = Texture::DataFormat::RED;
    texture.dataType = Texture::DataType::FLOAT;

    return texture;
}

} // namespace Engine
