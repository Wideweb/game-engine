#pragma once

#include "Framebuffer.hpp"
#include "Material.hpp"

namespace Engine {

struct RendererState {
    Framebuffer framebuffer;
    Material* baseMaterial;
};

} // namespace Engine