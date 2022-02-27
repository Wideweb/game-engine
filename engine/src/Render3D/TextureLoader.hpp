#pragma once

#include "Texture.hpp"

#include <stddef.h>
#include <string>

namespace Engine {

class TextureLoader {
  public:
    static Texture loadTexture(const std::string &path);
    static Texture placeholder();
    static Texture loadCubemap(const std::vector<std::string> &faces);
};

} // namespace Engine
