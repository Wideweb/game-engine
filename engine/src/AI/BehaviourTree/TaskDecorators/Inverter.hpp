#pragma once

#include "TaskDecorator.hpp"

namespace Engine {

class Inverter : public TaskDecorator {
  public:
    Inverter(Task *task) : TaskDecorator(task) {}

    virtual TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        if (m_Task->update(blackboard) == TaskStatus::Fail) {
            return TaskStatus::Success;
        }

        return TaskStatus::Fail;
    }
};

} // namespace Engine
