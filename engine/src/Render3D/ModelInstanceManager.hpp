#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "FlatDictionary.hpp"

namespace Engine {

class ModelInstanceManager {
  public:
    bool resized = false;
    bool updated = false;
    uint32_t from = 0;
    uint32_t to = 0;
    uint32_t staticEnd = 0;

    void create(uint32_t id, glm::mat4 position, bool isStatic) {
        m_Positions.add(id, position);
        m_Joints.emplace(id);

        if (isStatic) {
            staticEnd = m_Positions.size() - 1;
        }

        resized = true;
    }

    void update(uint32_t id, glm::mat4 position) {
        m_Positions[id] = position;
        auto index = m_Positions.getIndex(id);

        if (!updated) {
            from = index;
        }

        from = std::min(from, index);
        to = std::max(to, index);

        updated = true;
    }

    void update(uint32_t id, std::vector<glm::mat4> joints) {
        m_Joints.update(id, joints);
        auto index = m_Joints.getIndex(id);

        if (!updated) {
            from = index;
        }

        from = std::min(from, index);
        to = std::max(to, index);

        updated = true;
    }

    void remove(uint32_t id) {
        m_Positions.remove(id);
        m_Joints.remove(id);

        resized = true;
    }

    const glm::mat4 &position(uint32_t id) const { return m_Positions[id]; }
    const std::vector<glm::mat4> &joints(uint32_t id) const { return m_Joints[id]; }

    const std::vector<glm::mat4> &positions() const { return m_Positions.values(); }
    const std::vector<std::vector<glm::mat4>> &joints() const { return m_Joints.values(); }
    const std::vector<uint32_t> &ids() const { return m_Positions.keys(); }

    size_t size() { return m_Positions.size(); }

  private:
    FlatDictionary<uint32_t, glm::mat4> m_Positions;
    FlatDictionary<uint32_t, std::vector<glm::mat4>> m_Joints;
};

} // namespace Engine
