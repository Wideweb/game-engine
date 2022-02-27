#pragma once

namespace Engine {

class Viewport {
  public:
    unsigned int width;
    unsigned int height;

    void use();
    void resize(unsigned int width, unsigned int height);
};

} // namespace Engine