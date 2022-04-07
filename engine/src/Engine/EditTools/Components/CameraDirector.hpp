#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class CameraDirector : public BaseView {
  private:
    Entity m_Camera;
    GameObjectModel &m_Model;

  public:
    CameraDirector(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    bool handleSelection(Entity entity) override;

    void show() override;
    void hide() override;
};

} // namespace Engine
