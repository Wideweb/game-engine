#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"
#include "Particles.hpp"

namespace Engine {

class ParticlesPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    ParticlesConfiguration m_Particles;

  public:
    ParticlesPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
