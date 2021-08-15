#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class TransformPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    float m_Scale;

  public:
    TransformPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
