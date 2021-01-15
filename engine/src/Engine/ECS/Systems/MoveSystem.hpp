#pragma once

#include "System.hpp"

namespace Engine {

class MoveSystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
