#pragma once

#include "BaseView.hpp"

#include "Entity.hpp"
#include "Framebuffer.hpp"
#include "Renderbuffer.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#include <glm/vec2.hpp>

namespace Engine {

class ScenePanel : public BaseView {
  private:
    Framebuffer m_Framebuffer;
    Texture m_GColor, m_GEntity;
    Renderbuffer m_GDepth;
    glm::vec2 m_PrevViewportSize, m_ViewportSize;
    float m_Ratio;
    Entity m_Entity = c_NoEntity;
    bool m_BlockEvents = false;

  public:
    ScenePanel();

    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
    void onMouseEvent(MouseEvent &event) override;

    Entity entity() { return m_Entity; }
    bool blockEvents() { return m_BlockEvents; }
    glm::vec2 viewport() { return m_ViewportSize; }
};

} // namespace Engine
