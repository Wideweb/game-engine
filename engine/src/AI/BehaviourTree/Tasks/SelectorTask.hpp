#pragma once

#include <vector>

#include "Task.hpp"

namespace Engine {

class SelectorTask : public Task {
  private:
    int taskIndex = 0;
    std::vector<Task *> m_Tasks;

  public:
    SelectorTask(const std::vector<Task *> tasks) : m_Tasks(tasks) {}

    virtual ~SelectorTask() {
        for (auto task : m_Tasks) {
            delete task;
        }
        m_Tasks.clear();
    }

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        auto task = m_Tasks[taskIndex];

        TaskStatus taskStatus = task->update(blackboard);
        if (taskStatus == TaskStatus::Success) {
            taskIndex = 0;
            return TaskStatus::Success;
        }

        if (taskStatus == TaskStatus::Running) {
            return TaskStatus::Running;
        }

        if (taskIndex < m_Tasks.size() - 1) {
            taskIndex++;
            return TaskStatus::Running;
        }

        taskIndex = 0;
        return TaskStatus::Fail;
    }
};

} // namespace Engine
