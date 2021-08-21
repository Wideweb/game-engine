#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class VelocityPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    glm::vec3 m_Velocity = glm::vec3(0.0f);
    glm::vec3 m_Rotation = glm::vec3(0.0f);
    float m_Speed = 0.0f;

  public:
    VelocityPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
