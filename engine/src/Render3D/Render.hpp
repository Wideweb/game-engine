#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Camera.hpp"
#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

namespace Engine {

class Render {
  private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Shader> m_SkyboxShader;

  public:
    Render();

    void draw(Scene &scene, const ModelManager &models, const Camera &camera);
    void setClearColor(float r, float g, float b, float a);
    void clear();
};

} // namespace Engine
