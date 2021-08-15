#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include "Entity.hpp"

namespace Engine {

class SceneHierarchyPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    SceneHierarchyPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
