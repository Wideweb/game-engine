#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class Transform2DPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    Transform2DPanel(GameObjectModel &model);

    void onDraw() override;
};

} // namespace Engine
