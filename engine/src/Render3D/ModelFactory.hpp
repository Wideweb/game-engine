#pragma once

#include "InstancedModel.hpp"
#include "Model.hpp"

#include <glm/vec3.hpp>

#include <memory>

namespace Engine {

class ModelFactory {
  public:
    static std::shared_ptr<Model> createCube(float size = 1.0f);
    static std::shared_ptr<Model> createCube(float left, float right, float bottom, float top, float back, float front);
    static std::shared_ptr<InstancedModel> createPlane(float tileSize = 4.0f, int columns = 1, int rows = 1);
    static std::shared_ptr<Model> createCircle(float radius, int segments, float lineWidth = 0.1f,
                                               glm::vec3 color = glm::vec3(0.8f, 0.6f, 0.1f));
    static std::shared_ptr<Model> createFrastum(float fieldOfView, float nearPlane, float farPlane,
                                                glm::vec3 color = glm::vec3(0.8f, 0.6f, 0.1f));
};

} // namespace Engine
