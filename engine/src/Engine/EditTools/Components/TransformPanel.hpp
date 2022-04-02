#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class TransformPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    TransformPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
