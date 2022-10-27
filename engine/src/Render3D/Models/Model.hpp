#pragma once

#include "ModelInstanceBatch.hpp"
#include "Shader.hpp"

#include <vector>

namespace Engine {

class Model {
  public:
    virtual ~Model() {}

    virtual void setUp() = 0;
    virtual void update() = 0;
    virtual void draw(ModelInstanceBatch *batch, Shader* activeShader = nullptr) = 0;
};

} // namespace Engine