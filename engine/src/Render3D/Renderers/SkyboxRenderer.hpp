#pragma once

#include "Camera.hpp"
#include "RenderSettings.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

#include <memory>

namespace Engine {

class SkyboxRenderer {
  private:
    Shader m_Shader;

  public:
    SkyboxRenderer();
    ~SkyboxRenderer();

    void draw(Camera &camera, Scene &scene, RenderSettings &settings);
};

} // namespace Engine