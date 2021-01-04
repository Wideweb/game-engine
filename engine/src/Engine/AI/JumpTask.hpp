#pragma once

#include "Application.hpp"
#include "Layer.hpp"
#include "Task.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

class JumpTask : public Task {
  public:
    JumpTask() {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        const auto &layerName = blackboard->getValue<std::string>("layer");
        auto &layer = Application::get().getLayer(layerName);
        auto &coordinator = layer.getCoordinator();
        auto entity = blackboard->getValue<Entity>("entity");

        auto &velocity = coordinator.GetComponent<VelocityComponent>(entity);
        // velocity.velocity.y = blackboard->getValue<float>("speed_y");

        return TaskStatus::Success;
    }
};

} // namespace Engine
