#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class CollisionPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    bool m_Static;

    glm::vec3 m_Size;

  public:
    CollisionPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
