#pragma once

#include "Shader.hpp"
#include "ShaderModelInstanceManager.hpp"

#include <vector>

namespace Engine {

class Model {
  public:
    virtual ~Model() {}

    virtual void setUp() = 0;
    virtual void update() = 0;
    virtual void draw(Shader &shader, ShaderModelInstanceManager &instances) = 0;
};

} // namespace Engine