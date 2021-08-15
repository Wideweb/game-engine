#pragma once

#include <glm/mat4x4.hpp>
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

    ModelInstance Create(glm::mat4 position, bool isStatic, uint32_t id) {
        m_InstancesPositions.push_back(std::move(position));
        m_InstancesJoints.emplace_back();
        m_InstancesId.push_back(id);

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

    void Update(ModelInstance instance, std::vector<glm::mat4> joints) {
        size_t index = instance - 1;
        m_InstancesJoints[index] = std::move(joints);

        if (!update) {
            from = index;
        }

        from = std::min(from, index);
        to = std::max(to, index);

        update = true;
    }

    void Remove(ModelInstance instance) {
        size_t index = instance - 1;

        m_InstancesPositions[index] = m_InstancesPositions[m_InstancesPositions.size() - 1];
        m_InstancesJoints[index] = m_InstancesJoints[m_InstancesJoints.size() - 1];
        m_InstancesId[index] = m_InstancesId[m_InstancesId.size() - 1];

        m_InstancesPositions.pop_back();
        m_InstancesJoints.pop_back();

        resize = true;
    }

    const std::vector<glm::mat4> &GetPositions() { return m_InstancesPositions; }
    const std::vector<std::vector<glm::mat4>> &GetJoints() { return m_InstancesJoints; }
    const std::vector<uint32_t> &GetIds() { return m_InstancesId; }

    size_t size() { return m_InstancesPositions.size(); }

  private:
    std::vector<glm::mat4> m_InstancesPositions;
    std::vector<std::vector<glm::mat4>> m_InstancesJoints;
    std::vector<uint32_t> m_InstancesId;
};

} // namespace Engine
