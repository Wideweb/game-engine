#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

namespace Engine {

class Render {
  private:
    std::unique_ptr<Shader> shader;

  public:
    Render();

    void init();

    void draw(const Scene &scene, const Camera &camera);
    void setClearColor(float r, float g, float b, float a);
    void clear();
};

} // namespace Engine
