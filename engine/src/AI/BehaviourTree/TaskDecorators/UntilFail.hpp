#pragma once

#include "TaskDecorator.hpp"

namespace Engine {

class UntilFail : public TaskDecorator {
  public:
    UntilFail(Task *task) : TaskDecorator(task) {}

    virtual TaskStatus
    update(const std::shared_ptr<Blackboard> &blackboard) override {
        if (m_Task->update(blackboard) != TaskStatus::Fail) {
            return TaskStatus::Running;
        }

        return TaskStatus::Success;
    }
};

} // namespace Engine
