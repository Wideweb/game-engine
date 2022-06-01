#define STB_IMAGE_IMPLEMENTATION

#include "TextureLoader.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
#include "stb_image.hpp"
#pragma GCC diagnostic pop

#include "glad/glad.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace Engine {

Texture TextureLoader::m_Placeholder;

Texture TextureLoader::loadTexture(const std::string &path) {
    Texture texture;
    int width, height, channels;

    stbi_set_flip_vertically_on_load(0);

    unsigned char *data = stbi_load(path.data(), &width, &height, &channels, 0);

    assert(data && "file not found.");

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    } else if (channels == 1) {
        internalFormat = GL_R8;
        dataFormat = GL_RED;
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    GLint mipmapLevel = 0;
    GLint border = 0;

    // clang-format off
    glTexImage2D(GL_TEXTURE_2D,    // Specifies the target texture of the active texture unit
                 mipmapLevel,      // Specifies the level-of-detail number. Level 0 is the base image level
                 internalFormat,   // Specifies the internal format of the texture
                 static_cast<GLsizei>(width),
                 static_cast<GLsizei>(height),
                 border,           // Specifies the width of the border. Must be 0. For GLES 2.0
                 dataFormat,       // Specifies the format of the texel data. Must match internalformat
                 GL_UNSIGNED_BYTE, // Specifies the data type of the texel data
                 data);            // Specifies a pointer to the image data in memory
    // clang-format on

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    texture.width = width;
    texture.height = height;
    texture.type = Texture::TextureType::COLOR;

    if (internalFormat == GL_RGBA8) {
        texture.format = Texture::InternalFormat::RGBA8F;
        texture.dataFormat = Texture::DataFormat::RGBA;
    } else if (internalFormat == GL_RGB8) {
        texture.format = Texture::InternalFormat::RGB8F;
        texture.dataFormat = Texture::DataFormat::RGB;
    } else if (internalFormat == GL_R8) {
        texture.format = Texture::InternalFormat::R8F;
        texture.dataFormat = Texture::DataFormat::RED;
    }
    texture.dataType = Texture::DataType::UNSIGNED_BYTE;

    return texture;
}

Texture TextureLoader::placeholder() {
    if (!m_Placeholder.empty()) {
        return m_Placeholder;
    }

    Texture texture;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    unsigned char data[3] = {0, 0, 0};

    // clang-format off
    glTexImage2D(GL_TEXTURE_2D,    // Specifies the target texture of the active texture unit
                 0,      // Specifies the level-of-detail number. Level 0 is the base image level
                 GL_RGB8,   // Specifies the internal format of the texture
                 static_cast<GLsizei>(1),
                 static_cast<GLsizei>(1),
                 0,           // Specifies the width of the border. Must be 0. For GLES 2.0
                 GL_RGB,       // Specifies the format of the texel data. Must match internalformat
                 GL_UNSIGNED_BYTE, // Specifies the data type of the texel data
                 data);            // Specifies a pointer to the image data in memory
    // clang-format on

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = 0;
    texture.height = 0;
    texture.type = Texture::TextureType::COLOR;
    texture.format = Texture::InternalFormat::RGB8F;
    texture.dataFormat = Texture::DataFormat::RGB;
    texture.dataType = Texture::DataType::UNSIGNED_BYTE;

    m_Placeholder = texture;

    return texture;
}

CubeMapTexture TextureLoader::loadCubemap(const std::vector<std::string> &faces) {
    CubeMapTexture texture;

    GLint mipmapLevel = 0;
    GLint border = 0;
    int width, height, channels;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (size_t i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

        assert(data && "file not found.");

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        // clang-format off
        glTexImage2D(
            static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i),
            mipmapLevel,
            internalFormat,
            static_cast<GLsizei>(width),
            static_cast<GLsizei>(height),
            border,
            dataFormat,
            GL_UNSIGNED_BYTE,
            data);
        // clang-format on

        stbi_image_free(data);

        texture.faces[i] = loadTexture(faces[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    texture.width = width;
    texture.height = height;
    texture.type = Texture::TextureType::CUBE_MAP;
    texture.format = GL_RGB8 ? Texture::InternalFormat::RGB8F : Texture::InternalFormat::RGBA8F;
    texture.dataFormat = GL_RGB ? Texture::DataFormat::RGB : Texture::DataFormat::RGBA;
    texture.dataType = Texture::DataType::UNSIGNED_BYTE;

    return texture;
}

void TextureLoader::loadCubemap(CubeMapTexture &texture, std::string facePath, size_t faceIndex) {
    GLint mipmapLevel = 0;
    GLint border = 0;
    int width, height, channels;

    texture.bind();

    unsigned char *data = stbi_load(facePath.c_str(), &width, &height, &channels, 0);

    assert(data && "file not found.");

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    // clang-format off
    glTexImage2D(
        static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex),
        mipmapLevel,
        internalFormat,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        border,
        dataFormat,
        GL_UNSIGNED_BYTE,
        data);
    // clang-format on

    texture.width = width;
    texture.height = height;
    texture.type = Texture::TextureType::CUBE_MAP;
    texture.format = GL_RGB8 ? Texture::InternalFormat::RGB8F : Texture::InternalFormat::RGBA8F;
    texture.dataFormat = GL_RGB ? Texture::DataFormat::RGB : Texture::DataFormat::RGBA;
    texture.dataType = Texture::DataType::UNSIGNED_BYTE;

    texture.unbind();

    stbi_image_free(data);

    texture.faces[faceIndex] = loadTexture(facePath);
}

} // namespace Engine
