#pragma once

#include "System.hpp"

namespace Engine {

class Render3DSystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
