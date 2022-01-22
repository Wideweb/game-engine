#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"

namespace Engine {

class MeshBody : public BaseView {
  private:
    Entity m_MeshBody;
    GameObjectModel &m_Model;
    Render3DComponent m_Render;

    std::shared_ptr<Shader> m_Shader;

  public:
    MeshBody(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;

    void onDraw(int x, int y);

    void show() override;
    void hide() override;
};

} // namespace Engine
