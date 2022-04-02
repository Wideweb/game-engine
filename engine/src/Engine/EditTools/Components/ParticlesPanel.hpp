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
    
    void onDraw(int x, int y) override;
};

} // namespace Engine
