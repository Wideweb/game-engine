#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"
#include "Texture.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class DirectedLightDirector : public BaseView {
  private:
    Entity m_Sun;
    GameObjectModel &m_Model;
    Texture m_SunIcon;
    glm::vec3 m_SunPosition;
    bool m_SunSelected;

  public:
    DirectedLightDirector(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
    bool handleSelection(Entity entity) override;

    void show() override;
    void hide() override;
};

} // namespace Engine
