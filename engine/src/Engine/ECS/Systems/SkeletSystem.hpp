#pragma once

#include "BaseSystem.hpp"
#include "Skelet.hpp"

namespace Engine {

class SkeletSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;
    virtual void Update(ComponentManager &components) const override;

  private:
    size_t getFrame(const std::vector<JointAnimationKeyFrame> &frames, double time) const;
};

} // namespace Engine
