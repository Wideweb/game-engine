#pragma once

#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

namespace Engine {

class QuadRenderer {
  private:
    unsigned int m_QuadVAO, m_QuadVBO;

  public:
    QuadRenderer();

    void draw();
};

} // namespace Engine