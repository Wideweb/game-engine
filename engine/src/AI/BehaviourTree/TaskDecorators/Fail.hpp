#pragma once

#include "TaskDecorator.hpp"

namespace Engine {

class Fail : public TaskDecorator {
  public:
    Fail(Task *task) : TaskDecorator(task) {}

    virtual TaskStatus
    update(const std::shared_ptr<Blackboard> &blackboard) override {
        if (m_Task->update(blackboard) == TaskStatus::Running) {
            return TaskStatus::Running;
        }

        return TaskStatus::Fail;
    }
};

} // namespace Engine
