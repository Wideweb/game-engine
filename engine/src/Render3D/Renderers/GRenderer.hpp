#pragma once

#include "Camera.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SkyboxRenderer.hpp"
#include "Viewport.hpp"

#include <memory>

namespace Engine {

class GRenderer {
  private:
    std::unique_ptr<Shader> m_Shader;
    unsigned int m_GBuffer;
    unsigned int m_GPosition, m_GNormal, m_GColor, m_GSpecular, m_GDepth;

    Viewport &m_Viewport;
    ModelRenderer &m_ModelRenderer;
    SkyboxRenderer &m_SkyboxRenderer;

  public:
    GRenderer(Viewport &viewport, ModelRenderer &modelRenderer, SkyboxRenderer &skyboxRenderer);

    void draw(Camera &camera, Scene &scene, const ModelManager &models);
    void resize();

    unsigned int gPosition() { return m_GPosition; }
    unsigned int gNormal() { return m_GNormal; }
    unsigned int gColor() { return m_GColor; }
    unsigned int gDepth() { return m_GDepth; }
};

} // namespace Engine