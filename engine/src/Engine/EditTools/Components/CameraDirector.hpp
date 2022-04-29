#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Texture.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class CameraDirector : public BaseView {
  private:
    Entity m_Camera;
    GameObjectModel &m_Model;
    Texture m_CameraIcon;
    glm::vec3 m_CameraPosition;
    bool m_CameraSelected;

  public:
    CameraDirector(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
    bool handleSelection(Entity entity) override;

    void show() override;
    void hide() override;
};

} // namespace Engine
