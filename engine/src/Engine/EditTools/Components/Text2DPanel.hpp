#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class Text2DPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    Text2DPanel(GameObjectModel &model);

    void onDraw() override;
};

} // namespace Engine
