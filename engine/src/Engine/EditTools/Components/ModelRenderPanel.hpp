#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class ModelRenderPanel : public BaseView {
  private:
    GameObjectModel &m_Model;

    bool m_FreeDiffuseMap = false, m_FreeSpecularMap = false, m_FreeNormalMap = false, m_FreeMetallicMap = false,
         m_FreeRoughnessMap = false, m_FreeAmbientOcclusionMap = false;

  public:
    ModelRenderPanel(GameObjectModel &model);

    void onUpdate() override;
    void onDraw() override;
};

} // namespace Engine
