#pragma once

#include "System.hpp"

namespace Engine {

class LocationSystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
