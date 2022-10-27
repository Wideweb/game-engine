#pragma once

#include "BaseView.hpp"

#include "Framebuffer.hpp"
#include "Texture.hpp"

#include <glm/vec2.hpp>

namespace Engine {

class GamePanel : public BaseView {
  private:
    Framebuffer m_Framebuffer;
    RenderContext m_RenderContext;
    Texture m_GColor, m_GEntity, m_GDepth;
    glm::vec2 m_PrevViewportSize, m_ViewportSize;
    float m_Ratio;

  public:
    GamePanel();

    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
};

} // namespace Engine
