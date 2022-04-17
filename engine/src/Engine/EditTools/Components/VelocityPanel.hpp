#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class VelocityPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    VelocityPanel(GameObjectModel &model);

    void onDraw() override;
};

} // namespace Engine
