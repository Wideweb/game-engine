#pragma once

#include "System.hpp"

namespace Engine {

class AISystem : public System {
  public:
    virtual void Update(ComponentManager &components) override;
};

} // namespace Engine
