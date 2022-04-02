#pragma once

#include "BaseView.hpp"
#include "DirectedLight.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class DirectedLightPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    DirectedLightPanel(GameObjectModel &model);

    void onDraw(int x, int y) override;
};

} // namespace Engine
