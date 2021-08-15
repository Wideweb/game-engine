#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class BehaviourPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    BehaviourPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
