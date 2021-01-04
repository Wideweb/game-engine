#include "Texture.hpp"
#include "picopng.hpp"

namespace Engine {

Texture::Texture(const std::string &path) { load(path); }

bool Texture::load(const std::string &path) {
    std::vector<std::byte> fileInMemory;
    std::ifstream in(path.data(), std::ios_base::binary);

    assert(in && "file not found.");

    in.seekg(0, std::ios_base::end);
    size_t fileSize = static_cast<size_t>(in.tellg());
    fileInMemory.resize(fileSize);
    in.seekg(0, std::ios_base::beg);

    if (!in) {
        return false;
    }

    in.read(reinterpret_cast<char *>(fileInMemory.data()),
            static_cast<std::streamsize>(fileInMemory.size()));

    if (!in.good()) {
        return false;
    }

    std::vector<std::byte> image;
    unsigned long width = 0;
    unsigned long height = 0;
    int error = decodePNG(image, width, height, &fileInMemory[0],
                          fileInMemory.size(), false);

    if (error != 0) {
        std::cerr << "error: " << error << std::endl;
        return false;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    GLint mipmapLevel = 0;
    GLint border = 0;

    // clang-format off
    glTexImage2D(GL_TEXTURE_2D,    // Specifies the target texture of the active texture unit
                 mipmapLevel,      // Specifies the level-of-detail number. Level 0 is the base image level
                 GL_RGBA,          // Specifies the internal format of the texture
                 static_cast<GLsizei>(width),
                 static_cast<GLsizei>(height),
                 border,           // Specifies the width of the border. Must be 0. For GLES 2.0
                 GL_RGBA,          // Specifies the format of the texel data. Must match internalformat
                 GL_UNSIGNED_BYTE, // Specifies the data type of the texel data
                 image.data());       // Specifies a pointer to the image data in memory
    // clang-format on

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, m_TextureID); }

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

Texture::~Texture() { glDeleteTextures(1, &m_TextureID); }

} // namespace Engine
