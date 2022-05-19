#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"
#include "SpotLightComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class SpotLightPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    SpotLightPanel(GameObjectModel &model);

    void onDraw() override;
};

} // namespace Engine
