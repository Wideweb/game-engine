#pragma once

#include "Texture.hpp"

#include "glad/glad.h"

#include <stddef.h>
#include <string>

namespace Engine {

class TextureLoader {
  public:
    static Texture *loadTexture(const std::string &path);
    static Texture *loadTextureRGB(const std::string &path);
    static Texture *loadCubemap(const std::vector<std::string> &faces);
    static Texture *createDepthBuffer(int width, int height);
    static Texture *createCubeDepthBuffer(int width, int height);
    static Texture *createRGBA16Buffer(int width, int height);
    static Texture *createRGB16Buffer(int width, int height);
    static Texture *createRGB8IBuffer(int width, int height);
};

} // namespace Engine
