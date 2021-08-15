#pragma once

namespace Engine {

class QuadRenderer {
  private:
    unsigned int m_QuadVAO, m_QuadVBO;

  public:
    QuadRenderer();

    void draw();
};

} // namespace Engine