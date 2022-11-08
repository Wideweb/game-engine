#pragma once

#include "Material.hpp"
#include "Texture.hpp"
#include "Framebuffer.hpp"

namespace Engine {

struct RenderContext {
  Material baseMaterial;
  Texture brightnessColorBuffer, exposureColorBuffer[2];
  Framebuffer brightnessFramebuffer, exposureFramebuffer[2];
  int currentExposure = 0;

  ~RenderContext() {
    if (!brightnessColorBuffer.empty()) {
      brightnessColorBuffer.free();
    }

    if (!exposureColorBuffer[0].empty()) {
      exposureColorBuffer[0].free();
    }

    if (!exposureColorBuffer[1].empty()) {
      exposureColorBuffer[1].free();
    }

    if (!brightnessFramebuffer.empty()) {
      brightnessFramebuffer.free();
    }

    if (!exposureFramebuffer[0].empty()) {
      exposureFramebuffer[0].free();
    }

    if (!exposureFramebuffer[1].empty()) {
      exposureFramebuffer[1].free();
    }
  }
};

} // namespace Engine