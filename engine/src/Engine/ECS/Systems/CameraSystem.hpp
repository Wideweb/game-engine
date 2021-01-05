#pragma once

#include "System.hpp"

namespace Engine {

class CameraSystem : public System {
  public:
    virtual void Update(ComponentManager &components) const override;
};

} // namespace Engine
