#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "glad/glad.h"
#include <iostream>

namespace Engine {

Font::Font(const std::string &path) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    for (size_t i = 0; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        Texture texture = Texture::createTrueTypeGlyph(face->glyph->bitmap.width, face->glyph->bitmap.rows,
                                                       face->glyph->bitmap.buffer);

        Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x};
        characters.insert(std::pair<char, Character>(static_cast<char>(i), character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font() {
    for (auto &[k, v] : characters) {
        v.texture.free();
    }
    characters.clear();
}

} // namespace Engine