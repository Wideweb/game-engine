#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "GameObjectModel.hpp"
#include "Render3DComponent.hpp"
#include "Shader.hpp"
#include "TerrainPanel.hpp"

#include <memory>
#include <vector>

namespace Engine {

class TerrainTransform : public BaseView {
  private:
    GameObjectModel &m_Model;
    bool m_MouseDown = false;
    bool m_Edit = false;
    TerrainPanel &m_TerrainPanel;

    std::vector<glm::vec3> m_ModelVerticesColor;

    unsigned int m_VAO, m_VBO;

    std::shared_ptr<Shader> m_Shader;

    Entity m_Brush;

    glm::vec2 m_MousePos = glm::vec2(0.0f);

  public:
    TerrainTransform(GameObjectModel &model, TerrainPanel &terrainPanel);

    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
    void onMouseEvent(MouseEvent &event) override;
    void hide() override;
    void show() override;
};

} // namespace Engine
