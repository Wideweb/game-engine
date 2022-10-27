#pragma once

#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Material.hpp"
#include "RendererState.hpp"

namespace Engine {

class ModelRenderer {
  public:
    ModelRenderer();

    void draw(Scene &scene, const ModelManager &models, Material* baseMaterial, Shader* activeShader);
};

} // namespace Engine