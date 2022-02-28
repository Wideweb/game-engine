#pragma once

namespace Engine {

class QuadRenderer {
  private:
    unsigned int m_QuadVAO, m_QuadVBO;

  public:
    QuadRenderer();
    ~QuadRenderer();

    void draw();
};

} // namespace Engine