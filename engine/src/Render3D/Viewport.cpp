#include "Viewport.hpp"

#include "glad/glad.h"

namespace Engine {

void Viewport::use() { glViewport(0, 0, width, height); }

void Viewport::resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    use();
}

} // namespace Engine