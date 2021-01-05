#pragma once

#include <vector>

#include "Task.hpp"

namespace Engine {

class ParallelTask : public Task {
  private:
    std::vector<Task *> m_Tasks;

  public:
    ParallelTask(std::vector<Task *> tasks) : m_Tasks(std::move(tasks)) {}

    virtual ~ParallelTask() {
        for (auto task : m_Tasks) {
            delete task;
        }
        m_Tasks.clear();
    }

    TaskStatus update(const std::shared_ptr<Blackboard> &blackboard) override {
        TaskStatus result = TaskStatus::Fail;

        for (auto &task : m_Tasks) {

            TaskStatus taskStatus = TaskStatus::Running;
            do {
                taskStatus = task->update(blackboard);
            } while (taskStatus == TaskStatus::Running);

            if (taskStatus == TaskStatus::Success) {
                result = TaskStatus::Success;
            }
        }

        return result;
    }
};

} // namespace Engine
