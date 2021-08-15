#pragma once

#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class MaterialPanel : public BaseView {
  private:
    GameObjectModel &m_Model;
    float m_Specular, m_Shininess;

  public:
    MaterialPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
