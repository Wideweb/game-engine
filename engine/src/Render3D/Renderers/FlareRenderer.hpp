#pragma once

#include "Camera.hpp"
#include "QuadRenderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

#include <array>
#include <glm/glm.hpp>

namespace Engine {

class FlareRenderer {
  private:
    Shader m_LensShader;
    std::array<Texture, 11> m_LensFlares;
    std::array<float, 11> m_LensFlareSize;

    Viewport &m_Viewport;
    QuadRenderer &m_QuadRenderer;

  public:
    FlareRenderer(Viewport &viewport, QuadRenderer &quadRenderer);
    ~FlareRenderer();

    void draw(Camera &camera, const glm::vec3 &lightPosition);
};

} // namespace Engine