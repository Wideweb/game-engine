#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class ModelTransformPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    ModelTransformPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
