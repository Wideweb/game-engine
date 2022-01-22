#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"

namespace Engine {

class TerrainPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    int m_Columns = 1, m_Rows = 1;

  public:
    TerrainPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;

    float brushRadius = 1.0f;
    float brushStrength = 0.1f;
};

} // namespace Engine
