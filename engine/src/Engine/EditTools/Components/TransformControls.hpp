#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class TransformControls : public BaseView {
  private:
    GameObjectModel &m_Model;

    Entity m_ArrowX, m_ArrowY, m_ArrowZ;
    Entity m_ArcX, m_ArcY, m_ArcZ;
    Entity m_ModelAxisX, m_ModelAxisY, m_ModelAxisZ;
    Entity m_ActiveControl = c_NoEntity;

    glm::vec3 m_PrevMouseWorldPos;
    glm::vec2 m_InitMouseScreenPos;

    float m_InitScale;

  public:
    TransformControls(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
    void onDetach() override;
    void onMouseEvent(MouseEvent &event) override;
    bool handleSelection(Entity entity) override;
    void hide() override;

  private:
    void onTransform();
};

} // namespace Engine
