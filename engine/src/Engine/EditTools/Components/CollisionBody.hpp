#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"

namespace Engine {

class CollisionBody : public BaseView {
  private:
    Entity m_CollisionBody;
    GameObjectModel &m_Model;
    Render3DComponent m_Render;

    std::shared_ptr<Shader> m_Shader;

  public:
    CollisionBody(GameObjectModel &model);

    void onAttach() override;

    void onDraw(int x, int y) override;

    void show() override;
    void hide() override;
};

} // namespace Engine
