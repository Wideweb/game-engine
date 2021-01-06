#pragma once

#include "Application.hpp"
#include "Math.hpp"
#include "Task.hpp"
#include "VelocityComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class OnGroundTask : public Task {
  public:
    TaskStatus update(const std::shared_ptr<Blackboard> &blackboard) override {
        const auto &layerName = blackboard->getValue<std::string>("layer");
        auto &layer = Application::get().getLayer(layerName);
        auto &coordinator = layer.getCoordinator();
        auto entity = blackboard->getValue<Entity>("entity");

        auto &velocity = coordinator.GetComponent<VelocityComponent>(entity);

        if (Math::isEqual(velocity.velocity.y, 0.0f)) {
            return TaskStatus::Success;
        }

        return TaskStatus::Fail;
    }
};

} // namespace Engine
