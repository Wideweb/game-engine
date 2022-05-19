#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <glm/vec3.hpp>
#include <memory>

namespace Engine {

class SpotLightSceneTool : public BaseView {
  private:
    Entity m_Sun, m_SphereX, m_SphereY, m_SphereZ;
    GameObjectModel &m_Model;

    std::shared_ptr<Shader> m_Shader;

  public:
    SpotLightSceneTool(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;

    void show() override;
    void hide() override;
};

} // namespace Engine
