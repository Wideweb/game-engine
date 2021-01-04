#pragma once

#include "Application.hpp"
#include "Task.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

class MoveLeftTask : public Task {
  public:
    MoveLeftTask() {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        const auto &layerName = blackboard->getValue<std::string>("layer");
        auto &layer = Application::get().getLayer(layerName);
        auto &coordinator = layer.getCoordinator();
        auto entity = blackboard->getValue<Entity>("entity");

        auto &velocity = coordinator.GetComponent<VelocityComponent>(entity);
        // velocity.velocity.x = blackboard->getValue<float>("speed_x") * -1;

        return TaskStatus::Success;
    }
};

} // namespace Engine
