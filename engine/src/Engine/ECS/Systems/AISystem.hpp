#pragma once

#include "System.hpp"

namespace Engine {

class AISystem : public System {
  public:
    virtual const void Update(ComponentManager &components) override;
};

} // namespace Engine
