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
#include <memory>

namespace Engine {

class SpotLightRenderer {
  private:
    Shader m_CubeShadowShader;
    Framebuffer m_DepthCubeMapFramebuffer;
    std::array<std::unique_ptr<CubeMap>, 4> m_SadowCubeMaps;

    Viewport m_Viewport;
    ModelRenderer &m_ModelRenderer;

    unsigned int m_ActiveLights = 0;

  public:
    SpotLightRenderer(Viewport &viewport, ModelRenderer &modelRenderer);

    void apply(const SpotLight &light, const glm::vec3 &position, Shader &shader, Scene &scene,
               const ModelManager &models, RendererState &state);
    void clear();
};

} // namespace Engine