#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Material.hpp"

namespace Engine {

class MeshBody : public BaseView {
  private:
    Entity m_MeshBody;
    GameObjectModel &m_Model;

  public:
    MeshBody(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;

    void show() override;
    void hide() override;
};

} // namespace Engine
