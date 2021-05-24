#pragma once

#include "Camera.hpp"
#include "QuadRenderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

#include <array>
#include <glm/glm.hpp>
#include <memory>

namespace Engine {

class FlareRenderer {
  private:
    std::unique_ptr<Shader> m_LensShader;
    std::array<std::shared_ptr<Texture>, 11> m_LensFlares;
    std::array<float, 11> m_LensFlareSize;

    Viewport &m_Viewport;
    QuadRenderer &m_QuadRenderer;

  public:
    FlareRenderer(Viewport &viewport, QuadRenderer &quadRenderer);

    void draw(Camera &camera, const glm::vec3 &lightPosition);
};

} // namespace Engine