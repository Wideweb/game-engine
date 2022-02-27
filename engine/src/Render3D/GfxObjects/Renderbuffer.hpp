#pragma once

#include "GfxImage.hpp"

namespace Engine {

class Renderbuffer : public GfxImage {
  public:
    void bind() const override;
    void unbind() const override;
    void resize(unsigned int width, unsigned int height) override;

    static Renderbuffer create(unsigned int width, unsigned int height, InternalFormat format);
};

} // namespace Engine