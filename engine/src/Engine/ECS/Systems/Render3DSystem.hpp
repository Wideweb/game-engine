#pragma once

#include "BaseSystem.hpp"
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
    glm::mat4x4 GetTransform(const Render3DComponent &render, const LocationComponent location) const;
};

} // namespace Engine
