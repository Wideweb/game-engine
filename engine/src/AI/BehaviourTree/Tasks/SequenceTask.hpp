#pragma once

#include <vector>

#include "Task.hpp"

namespace Engine {

class SequenceTask : public Task {
  private:
    size_t taskIndex = 0;
    std::vector<Task *> m_Tasks;

  public:
    SequenceTask(std::vector<Task *> tasks) : m_Tasks(std::move(tasks)) {}

    virtual ~SequenceTask() {
        for (auto task : m_Tasks) {
            delete task;
        }
        m_Tasks.clear();
    }

    TaskStatus update(const std::shared_ptr<Blackboard> &blackboard) override {
        auto task = m_Tasks[taskIndex];

        TaskStatus taskStatus = task->update(blackboard);
        if (taskStatus == TaskStatus::Fail) {
            return TaskStatus::Fail;
        }

        if (taskStatus == TaskStatus::Running) {
            return TaskStatus::Running;
        }

        if (taskIndex < m_Tasks.size() - 1) {
            taskIndex++;
            return TaskStatus::Running;
        }

        taskIndex = 0;
        return TaskStatus::Success;
    }
};

} // namespace Engine
