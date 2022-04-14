#pragma once

#include "BaseSystem.hpp"
#include "ComponentManager.hpp"
#include "LocationComponent.hpp"
#include "Render3DComponent.hpp"

#include <glm/mat4x4.hpp>
#include <string>

namespace Engine {

class Render3DSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;

    virtual void Attach(ComponentManager &components) const override;
    virtual void Update(ComponentManager &components) const override;

  private:
    void OnRemoveComponent(Entity entity) const;
    void OnModelChange(Entity entity, Render3DComponent &render) const;
};

} // namespace Engine
