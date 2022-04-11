#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class ModelRenderPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    ModelRenderPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
