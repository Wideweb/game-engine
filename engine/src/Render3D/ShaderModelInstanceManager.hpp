#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "FlatDictionary.hpp"
#include "ModelInstanceManager.hpp"
#include "Shader.hpp"

namespace Engine {

class ShaderModelInstanceManager {
  public:
    void create(uint32_t id, glm::mat4 position, bool isStatic) {
        m_InstanceShader.add(id, nullptr);

        if (!m_ShaderInstances.hasKey(c_NoShader)) {
            m_ShaderInstances.emplace(c_NoShader);
        }

        m_ShaderInstances[c_NoShader].create(id, position, isStatic);
    }

    void update(uint32_t id, glm::mat4 position) { m_ShaderInstances[getShaderId(id)].update(id, position); }

    void update(uint32_t id, std::vector<glm::mat4> joints) { m_ShaderInstances[getShaderId(id)].update(id, joints); }

    void update(uint32_t id, std::shared_ptr<Shader> shader) {
        auto prevShaderId = getShaderId(id);
        auto nextShaderId = shader ? shader->id : c_NoShader;
        if (prevShaderId == nextShaderId) {
            return;
        }

        m_InstanceShader[id] = shader;
        if (!m_ShaderInstances.hasKey(nextShaderId)) {
            m_ShaderInstances.emplace(nextShaderId);
        }

        auto &prevInstances = m_ShaderInstances[prevShaderId];
        glm::mat4 position = prevInstances.position(id);
        std::vector<glm::mat4> joints = prevInstances.joints(id);

        prevInstances.remove(id);

        auto &nextInstances = m_ShaderInstances[nextShaderId];
        nextInstances.create(id, position, false);
        nextInstances.update(id, joints);
    }

    void remove(uint32_t id) {
        auto shaderId = getShaderId(id);
        auto &instances = m_ShaderInstances[shaderId];
        instances.remove(id);

        m_InstanceShader.remove(id);

        // if (instances.size() == 0) {
        //     m_ShaderInstances.remove(shaderId);
        // }
    }

    ModelInstanceManager &instances(ShaderId shaderId) { return m_ShaderInstances[shaderId]; }

    const std::vector<std::shared_ptr<Shader>> &shaders() const { return m_InstanceShader.values(); }

    const std::vector<glm::mat4> &positions(ShaderId shaderId) const { return m_ShaderInstances[shaderId].positions(); }
    const std::vector<std::vector<glm::mat4>> &joints(ShaderId shaderId) const {
        return m_ShaderInstances[shaderId].joints();
    }
    const std::vector<uint32_t> &ids(ShaderId shaderId) const { return m_ShaderInstances[shaderId].ids(); }

    size_t size() { return m_ShaderInstances.size(); }

  private:
    ShaderId getShaderId(uint32_t id) const {
        if (m_InstanceShader[id] != nullptr) {
            return m_InstanceShader[id]->id;
        }

        return c_NoShader;
    }

    FlatDictionary<uint32_t, std::shared_ptr<Shader>> m_InstanceShader;
    FlatDictionary<ShaderId, ModelInstanceManager> m_ShaderInstances;
};

} // namespace Engine
