#pragma once

#include "System.hpp"

namespace Engine {

class Light3DSystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
