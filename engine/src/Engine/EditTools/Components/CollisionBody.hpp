#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"
#include "Shader.hpp"
#include "Material.hpp"

namespace Engine {

class CollisionBody : public BaseView {
  private:
    Entity m_CollisionBody;
    GameObjectModel &m_Model;

  public:
    CollisionBody(GameObjectModel &model);

    void onAttach() override;

    void show() override;
    void hide() override;
};

} // namespace Engine
