#pragma once

#include "System.hpp"

namespace Engine {

class MoveSystem : public System {
  public:
    virtual void Update(ComponentManager &components) override;
};

} // namespace Engine
