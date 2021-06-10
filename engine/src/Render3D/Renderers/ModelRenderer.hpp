#pragma once

#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "RendererState.hpp"

namespace Engine {

class ModelRenderer {
  public:
    ModelRenderer();

    void draw(Shader &shader, Scene &scene, const ModelManager &models, RendererState& state);
};

} // namespace Engine