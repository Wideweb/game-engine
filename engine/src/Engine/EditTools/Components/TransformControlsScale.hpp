#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class TransformControlsScale : public BaseView {
  private:
    GameObjectModel &m_Model;

    Entity m_ControlX, m_ControlY, m_ControlZ;
    Entity m_ActiveControl = c_NoEntity, m_HoveredControl = c_NoEntity;

    glm::vec3 m_PrevMouseWorldPos;
    glm::vec2 m_InitMouseScreenPos;

    glm::vec3 m_InitScale;

  public:
    TransformControlsScale(GameObjectModel &model);

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
