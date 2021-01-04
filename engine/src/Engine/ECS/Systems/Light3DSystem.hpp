#pragma once

#include "System.hpp"

namespace Engine {

class Light3DSystem : public System {
  public:
    virtual const void Update(ComponentManager &components) override;
};

} // namespace Engine
