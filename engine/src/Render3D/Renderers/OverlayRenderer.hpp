#pragma once

#include "Camera.hpp"
#include "ModelRenderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

#include <memory>

namespace Engine {

class OverlayRenderer {
  private:
    std::unique_ptr<Shader> m_Shader;

  public:
    OverlayRenderer();

    void draw(Camera &camera, Scene &scene, const ModelManager &models);
};

} // namespace Engine