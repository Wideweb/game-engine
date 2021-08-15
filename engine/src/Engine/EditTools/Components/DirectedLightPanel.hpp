#pragma once

#include "BaseView.hpp"
#include "DirectedLight.hpp"
#include "DirectedLightModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class DirectedLightPanel : public BaseView {
  private:
    DirectedLightModel &m_Model;

    DirectedLight m_Light;

  public:
    DirectedLightPanel(DirectedLightModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
