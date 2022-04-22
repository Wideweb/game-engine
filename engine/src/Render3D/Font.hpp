#pragma once

#include "Texture.hpp"

#include "glm/vec2.hpp"
#include <map>
#include <string>

namespace Engine {

class Font {
  public:
    struct Character {
        Texture texture;
        glm::ivec2 size;    // Glyph Size
        glm::ivec2 bearing; // Glyph Top Left Offset
        long advance;       // Horizontal offset to start of next glyph
    };

    std::map<char, Character> characters;

    Font(const std::string &path);
    ~Font();
};

} // namespace Engine