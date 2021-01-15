#pragma once

#include "System.hpp"

namespace Engine {

class AISystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
