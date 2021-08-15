#include "InstancedModel.hpp"

namespace Engine {

InstancedModel::InstancedModel() {}

InstancedModel::InstancedModel(const std::vector<InstancedMesh> &meshes) : meshes(meshes) {}

void InstancedModel::setUp() {
    for (auto &mesh : meshes) {
        mesh.setUp();
    }
}

void InstancedModel::update() {
    for (auto &mesh : meshes) {
        mesh.update();
    }
}

void InstancedModel::draw(Shader &shader, ModelInstanceManager &instances, unsigned int textureShift) {
    for (const auto &mesh : meshes) {
        if (instances.resize) {
            mesh.setInstances(instances.GetPositions(), instances.GetIds());
        } else if (instances.update) {
            mesh.updateInstances(instances.from, instances.to, instances.GetPositions(), instances.GetIds());
        }

        mesh.draw(shader, instances.size(), textureShift);
    }

    instances.resize = false;
    instances.update = false;
}

} // namespace Engine
