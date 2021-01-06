#pragma once

#include "Application.hpp"
#include "Task.hpp"
#include "VelocityComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class JumpTask : public Task {
  private:
    float m_Speed;

  public:
    JumpTask(float speed) : m_Speed(speed) {}

    TaskStatus update(const std::shared_ptr<Blackboard> &blackboard) override {
        const auto &layerName = blackboard->getValue<std::string>("layer");
        auto &layer = Application::get().getLayer(layerName);
        auto &coordinator = layer.getCoordinator();
        auto entity = blackboard->getValue<Entity>("entity");

        auto &velocity = coordinator.GetComponent<VelocityComponent>(entity);
        velocity.velocity.y = m_Speed;

        return TaskStatus::Success;
    }
};

} // namespace Engine
