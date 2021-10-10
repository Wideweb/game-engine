#pragma once

#include "Texture.hpp"

#include "glad/glad.h"

#include <stddef.h>
#include <string>

namespace Engine {

class TextureLoader {
  public:
    static Texture *loadTexture(const std::string &path);
    static Texture *placeholder();
    static Texture *loadCubemap(const std::vector<std::string> &faces);
    static Texture *createDepthBuffer(int width, int height);
    static Texture *createCubeDepthBuffer(int width, int height);
    static Texture *createRGBA32Buffer(int width, int height, const void *data = NULL);
    static Texture *createRGBA16Buffer(int width, int height);
    static Texture *createRGB16Buffer(int width, int height);
    static Texture *createRGB8IBuffer(int width, int height);
    static Texture *createRGBA8Buffer(int width, int height);
    static Texture *createR32IBuffer(int width, int height);
};

} // namespace Engine
