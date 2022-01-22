#pragma once

#include "ShaderModelInstanceManager.hpp"
#include "Shader.hpp"

#include <vector>

namespace Engine {

class Model {
  public:
    virtual ~Model() {}

    virtual void setUp() = 0;
    virtual void update() = 0;
    virtual void draw(Shader &shader, ShaderModelInstanceManager &instances, unsigned int textureShift) = 0;
};

} // namespace Engine