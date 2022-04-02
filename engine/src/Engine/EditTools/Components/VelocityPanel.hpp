#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class VelocityPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    VelocityPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
