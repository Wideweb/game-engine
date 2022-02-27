#include "Renderbuffer.hpp"

#include "glad/glad.h"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace Engine {

Renderbuffer Renderbuffer::create(unsigned int width, unsigned int height, InternalFormat format) {
    Renderbuffer renderbuffer;
    renderbuffer.width = width;
    renderbuffer.height = height;
    renderbuffer.format = format;

    glGenRenderbuffers(1, &renderbuffer.id);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer.id);
    glRenderbufferStorage(GL_RENDERBUFFER, GfxImage::getNativeFormat(format), width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return renderbuffer;
}

void Renderbuffer::bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id); }

void Renderbuffer::unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

void Renderbuffer::resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;

    glRenderbufferStorage(GL_RENDERBUFFER, GfxImage::getNativeFormat(format), width, height);
}

} // namespace Engine