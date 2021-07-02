#pragma once

#include "Engine.hpp"

#include <filesystem>
#include <glm/vec3.hpp>

class EditToolsLayer : public Engine::Layer {
  private:
    Engine::Entity m_ArrowX, m_ArrowY, m_ArrowZ;
    Engine::Entity m_ActiveEntity = Engine::c_NoEntity;
    Engine::Entity m_ActiveArrow = Engine::c_NoEntity;

    bool m_MousePressed = false;

    glm::vec3 m_PrevMousePosition;

    Engine::Layer &gameLayer();

  public:
    using Layer::Layer;

    virtual void onAttach() override;
    virtual void onUpdate() override;
    virtual void onDetach() override;
    virtual void onMouseEvent(Engine::MouseEvent &event) override;
};
