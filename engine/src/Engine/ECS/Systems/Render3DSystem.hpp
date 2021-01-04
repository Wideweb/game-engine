#pragma once

#include "System.hpp"

namespace Engine {

class Render3DSystem : public System {
  public:
    virtual const void Update(ComponentManager &components) override;
};

} // namespace Engine
