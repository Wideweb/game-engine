#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"

namespace Engine {

class DirectedLightDirector : public BaseView {
  private:
    Entity m_Sun;
    GameObjectModel &m_Model;

  public:
    DirectedLightDirector(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    bool handleSelection(Entity entity) override;

    void show() override;
    void hide() override;
};

} // namespace Engine
