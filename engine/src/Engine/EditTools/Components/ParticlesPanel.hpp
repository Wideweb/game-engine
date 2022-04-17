#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"
#include "Particles.hpp"

namespace Engine {

class ParticlesPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

  public:
    ParticlesPanel(GameObjectModel &model);

    void onDraw() override;
};

} // namespace Engine
