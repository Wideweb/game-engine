#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class DirectedLightDirector : public BaseView {
  private:
    Entity m_Sun;
    GameObjectModel &m_Model;

  public:
    DirectedLightDirector(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;

    void show() override;
    void hide() override;
};

} // namespace Engine
