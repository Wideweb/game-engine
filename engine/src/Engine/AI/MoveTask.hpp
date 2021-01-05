#pragma once

#include "Application.hpp"
#include "Task.hpp"
#include "VelocityComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class MoveTask : public Task {
  private:
    float m_Speed;

  public:
    MoveTask(float speed) : m_Speed(speed) {}

    TaskStatus update(const std::shared_ptr<Blackboard> &blackboard) override {
        const auto &layerName = blackboard->getValue<std::string>("layer");
        auto &layer = Application::get().getLayer(layerName);
        auto &coordinator = layer.getCoordinator();
        auto entity = blackboard->getValue<Entity>("entity");

        auto &velocity = coordinator.GetComponent<VelocityComponent>(entity);
        velocity.speed = m_Speed;

        return TaskStatus::Success;
    }
};

} // namespace Engine
