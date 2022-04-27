#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"

namespace Engine {

class MeshBody : public BaseView {
  private:
    Entity m_MeshBody;
    GameObjectModel &m_Model;

    std::shared_ptr<Shader> m_Shader;

  public:
    MeshBody(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;

    void show() override;
    void hide() override;
};

} // namespace Engine
