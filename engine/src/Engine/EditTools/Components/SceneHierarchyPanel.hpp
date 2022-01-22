#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include "Entity.hpp"

#include <string>
#include <vector>

namespace Engine {

class SceneHierarchyNode {
  public:
    std::string title;
    Entity entity;
    std::vector<SceneHierarchyNode *> children;

    void display(GameObjectModel &model, Coordinator &coordinator, bool childOnly = false) const;
    void addTools(GameObjectModel &model, Coordinator &coordinator) const;
};

class SceneHierarchyPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    SceneHierarchyPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
