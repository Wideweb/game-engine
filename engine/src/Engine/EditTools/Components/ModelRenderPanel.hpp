#pragma once

#include "BaseView.hpp"
#include "GLMetaData.hpp"
#include "GameObjectModel.hpp"
#include "Material.hpp"
#include "Shader.hpp"

namespace Engine {

class ModelRenderPanel : public BaseView {
  private:
    GameObjectModel &m_Model;
    float m_Padding = 10.0f;

  public:
    ModelRenderPanel(GameObjectModel &model);

    void onDraw() override;

  private:
    void showMaterialProperties(Material *material);
    void showTextureProperty(Material *material, const std::string &propertyName);
    void showIntProperty(Material *material, const std::string &propertyName);
    void showFloatProperty(Material *material, const std::string &propertyName);
    void showFloat3Property(Material *material, const std::string &propertyName);
    GLPropertyMetaData *getPropertyMetaData(std::string property, Shader *shader);
};

} // namespace Engine
