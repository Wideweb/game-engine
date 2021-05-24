#pragma once

#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

#include <memory>

namespace Engine {

class SkyboxRenderer {
  private:
    std::unique_ptr<Shader> m_SkyboxShader;
    float m_SkyboxRotation = 0.0f;

  public:
    SkyboxRenderer();

    void draw(Camera &camera, Scene &scene);
};

} // namespace Engine