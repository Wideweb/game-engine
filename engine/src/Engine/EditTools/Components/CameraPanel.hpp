#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <string>

namespace Engine {

class CameraPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    CameraPanel(GameObjectModel &model);

    void onDraw() override;
};

} // namespace Engine
