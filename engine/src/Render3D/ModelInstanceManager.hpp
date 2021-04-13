#pragma once

#include <vector>

namespace Engine {

using ModelInstance = uint32_t;

constexpr ModelInstance NoModelInstance = 0;

class ModelInstanceManager {
  public:
    bool resize = false;
    bool update = false;
    size_t from = 0;
    size_t to = 0;
    size_t staticEnd = 0;

    ModelInstance Create(glm::mat4 position, bool isStatic) {
        m_InstancesPositions.push_back(std::move(position));

        if (isStatic) {
            staticEnd = m_InstancesPositions.size() - 1;
        }

        resize = true;

        return static_cast<ModelInstance>(m_InstancesPositions.size());
    }

    void Update(ModelInstance instance, glm::mat4 position) {
        size_t index = instance - 1;
        m_InstancesPositions[index] = std::move(position);

        if (!update) {
            from = index;
        }

        from = std::min(from, index);
        to = std::max(to, index);

        update = true;
    }

    const std::vector<glm::mat4> &GetData() { return m_InstancesPositions; }

  private:
    std::vector<glm::mat4> m_InstancesPositions;
};

} // namespace Engine
