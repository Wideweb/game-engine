#pragma once

#include "Texture.hpp"

#include <stddef.h>
#include <string>

namespace Engine {

class TextureLoader {
  private:
    static Texture m_Placeholder;

  public:
    static Texture loadTexture(const std::string &path);
    static Texture placeholder();
    static CubeMapTexture loadCubemap(const std::vector<std::string> &faces);
    static void loadCubemap(CubeMapTexture &texture, std::string facePath, size_t faceIndex);
};

} // namespace Engine
