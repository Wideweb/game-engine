#pragma once

#include "BaseView.hpp"
#include "DirectedLight.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class DirectedLightPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    DirectedLight m_Light;

  public:
    DirectedLightPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
