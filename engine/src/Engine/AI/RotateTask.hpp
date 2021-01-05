#pragma once

#include "Application.hpp"
#include "Task.hpp"
#include "VelocityComponent.hpp"

namespace Engine {

class RotateTask : public Task {
  private:
    glm::vec3 m_Rotation;

  public:
    RotateTask(glm::vec3 rotation) : m_Rotation(rotation) {}

    TaskStatus update(const std::shared_ptr<Blackboard> &blackboard) override {
        const auto &layerName = blackboard->getValue<std::string>("layer");
        auto &layer = Application::get().getLayer(layerName);
        auto &coordinator = layer.getCoordinator();
        auto entity = blackboard->getValue<Entity>("entity");

        auto &velocity = coordinator.GetComponent<VelocityComponent>(entity);
        velocity.rotation = m_Rotation;

        return TaskStatus::Success;
    }
};

} // namespace Engine
