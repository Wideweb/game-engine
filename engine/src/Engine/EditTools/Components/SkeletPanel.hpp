#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <string>

namespace Engine {

class SkeletPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    SkeletPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
};

} // namespace Engine
