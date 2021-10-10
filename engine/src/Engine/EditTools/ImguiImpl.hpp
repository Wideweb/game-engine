#pragma once

#include "Application.hpp"
#include "Entity.hpp"

#include "imgui/imgui.h"
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Texture.hpp"

namespace Engine {

class ImguiImpl {
  private:
    unsigned int m_FBO, m_GColor, m_GEntity, m_GDepth;
    std::shared_ptr<Texture> m_ColorBuffer[2];

    std::shared_ptr<Texture> m_ImGuiFonts;
    std::shared_ptr<Texture> m_IconPlay, m_IconStop;
    glm::vec2 m_PrevViewportSize, m_ViewportSize;
    float m_Ratio;
    bool m_BlockEvents = true;

    Entity m_Entity;
    glm::vec3 m_CameraPos;
    glm::vec3 m_CameraRotation;

    Layer &layer() { return Application::get().getLayer("game_area"); }
    Coordinator &coordinator() { return layer().getCoordinator(); }

  public:
    void OnAttach();
    void onUpdate();
    void OnDetach();
    void onMouseEvent(MouseEvent &event);
    void Begin();
    void End();

    glm::vec2 Viewport() { return m_ViewportSize; }
    bool BlockEvents() { return m_BlockEvents; }
    Entity entity() { return m_Entity; }
};

} // namespace Engine
