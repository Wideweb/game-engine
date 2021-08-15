#pragma once

#include "BaseView.hpp"

#include "Texture.hpp"

#include <glm/vec2.hpp>

namespace Engine {

class GamePanel : public BaseView {
  private:
    unsigned int m_FBO, m_GColor, m_GEntity, m_GDepth;
    std::shared_ptr<Texture> m_ColorBuffer[2];
    glm::vec2 m_PrevViewportSize, m_ViewportSize;
    float m_Ratio;

  public:
    GamePanel();

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
