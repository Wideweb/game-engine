#pragma once

#include "CubeMap.hpp"
#include "Framebuffer.hpp"
#include "ModelManager.hpp"
#include "ModelRenderer.hpp"
#include "RendererState.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SpotLight.hpp"
#include "Viewport.hpp"

#include <glm/glm.hpp>

namespace Engine {

class SpotLightRenderer {
  private:
    Shader m_CubeShadowShader;
    Framebuffer m_DepthCubeMapFramebuffer;
    std::array<CubeMap, 4> m_SadowCubeMaps;

    Viewport &m_Viewport;
    ModelRenderer &m_ModelRenderer;

    unsigned int m_ActiveLights = 0;

  public:
    SpotLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer);
    ~SpotLightRenderer();

    void apply(const SpotLight &light, const glm::vec3 &position, Scene &scene,
               const ModelManager &models, RendererState &state);
    void clear();
};

} // namespace Engine