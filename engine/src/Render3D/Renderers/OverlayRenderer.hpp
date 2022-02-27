#pragma once

#include "Camera.hpp"
#include "ModelRenderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

namespace Engine {

class OverlayRenderer {
  private:
    Shader m_Shader;

  public:
    OverlayRenderer();

    void draw(Camera &camera, Scene &scene, const ModelManager &models);
};

} // namespace Engine