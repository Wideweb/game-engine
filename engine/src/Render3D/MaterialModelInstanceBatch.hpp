#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "FlatDictionary.hpp"
#include "ModelInstanceBatch.hpp"
#include "Material.hpp"

namespace Engine {

class MaterialModelInstanceBatch {
  public:
    void create(uint32_t id, const glm::mat4& position, bool isStatic, Material* material) {
        m_InstanceMaterial.add(id, material);

        if (!m_MaterialInstances.hasKey(material)) {
            m_MaterialInstances.add(material, std::make_shared<ModelInstanceBatch>(m_MaterialInstances.size()));
        }

        m_MaterialInstances[material]->create(id, position, isStatic);
    }

    void update(uint32_t id, const glm::mat4& position) { m_MaterialInstances[m_InstanceMaterial[id]]->update(id, position); }

    void update(uint32_t id, const std::vector<glm::mat4>& joints) { m_MaterialInstances[m_InstanceMaterial[id]]->update(id, joints); }

    void update(uint32_t id, Material* material) {
        if (material == m_InstanceMaterial[id]) {
            return;
        }

        m_InstanceMaterial[id] = material;
        if (!m_MaterialInstances.hasKey(material)) {
            m_MaterialInstances.add(material, std::make_shared<ModelInstanceBatch>(m_MaterialInstances.size()));
        }

        auto& prevInstances = m_MaterialInstances[material];
        auto& position = prevInstances->position(id);
        auto& joints = prevInstances->joints(id);

        prevInstances->remove(id);

        auto& nextInstances = m_MaterialInstances[m_InstanceMaterial[id]];
        nextInstances->create(id, position, false);
        nextInstances->update(id, joints);
    }

    void remove(uint32_t id) {
        auto& instances = m_MaterialInstances[m_InstanceMaterial[id]];
        instances->remove(id);
        m_InstanceMaterial.remove(id);
    }

    ModelInstanceBatch* instances(Material* material) {
        return m_MaterialInstances[material].get();
    }

    const std::vector<Material*> &materials() const {
        return m_InstanceMaterial.values();
    }

    const std::vector<glm::mat4> &positions(Material* material) const {
        return m_MaterialInstances[material]->positions();
    }

    const std::vector<std::vector<glm::mat4>> &joints(Material* material) const {
        return m_MaterialInstances[material]->joints();
    }
    
    const std::vector<uint32_t> &ids(Material* material) const { return m_MaterialInstances[material]->ids(); }

    size_t size() { return m_MaterialInstances.size(); }

  private:
    FlatDictionary<uint32_t, Material*> m_InstanceMaterial;
    FlatDictionary<Material*, std::shared_ptr<ModelInstanceBatch>> m_MaterialInstances;
};

} // namespace Engine
