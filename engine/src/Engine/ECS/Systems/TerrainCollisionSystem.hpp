#pragma once

#include "BaseSystem.hpp"
#include "Collision3D.hpp"
#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "LocationComponent.hpp"
#include "Render3DComponent.hpp"

namespace Engine {

class TerrainCollisionSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;

    glm::mat4x4 GetTransform(Entity entity, ComponentManager &components, const Render3DComponent &render,
                             const LocationComponent location) const;
};

} // namespace Engine
