#pragma once

#include <memory>

#include "Camera.hpp"
#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

#include "DirectedLightRenderer.hpp"
#include "FlareRenderer.hpp"
#include "GRenderer.hpp"
#include "ModelRenderer.hpp"
#include "ParticlesRenderer.hpp"
#include "QuadRenderer.hpp"
#include "SkyboxRenderer.hpp"
#include "SpotLightRenderer.hpp"
#include "WaterRenderer.hpp"

#include <memory>

namespace Engine {

class MasterRenderer {
  private:
    std::unique_ptr<Shader> m_Shader;

    std::unique_ptr<QuadRenderer> m_QuadRenderer;
    std::unique_ptr<ModelRenderer> m_ModelRenderer;
    std::unique_ptr<SkyboxRenderer> m_SkyboxRenderer;
    std::unique_ptr<DirectedLightRenderer> m_DirectedLightRenderer;
    std::unique_ptr<SpotLightRenderer> m_SpotLightRenderer;
    std::unique_ptr<WaterRenderer> m_WaterRenderer;
    std::unique_ptr<FlareRenderer> m_FlareRenderer;
    std::unique_ptr<GRenderer> m_GRenderer;
    std::unique_ptr<ParticlesRenderer> m_ParticlesRenderer;

    Viewport m_Viewport;

  public:
    MasterRenderer(int width, int height);

    void draw(Camera &camera, Scene &scene, const ModelManager &models);
    void setClearColor(float r, float g, float b, float a);
    void setViewport(int width, int height);
    void clear();
};

} // namespace Engine