#include "SkinnedModel.hpp"

namespace Engine {

SkinnedModel::SkinnedModel() {}

SkinnedModel::SkinnedModel(const std::vector<SkinnedMesh> &meshes) : meshes(meshes) {}

void SkinnedModel::setUp() {
    for (auto &mesh : meshes) {
        mesh.setUp();
    }
}

void SkinnedModel::update() {
    for (auto &mesh : meshes) {
        mesh.update();
    }
}

void SkinnedModel::draw(ModelInstanceBatch* batch, Shader* activeShader) {
    for (const auto &mesh : meshes) {
        for (size_t i = 0; i < batch->size(); i++) {
            auto position = batch->positions()[i];
            auto joints = batch->joints()[i];
            auto id = batch->ids()[i];

            activeShader->setInt("u_jointsNumber", static_cast<int>(joints.size()));
            for (size_t i = 0; i < joints.size(); i++) {
                activeShader->setMatrix4("u_joints[" + std::to_string(i) + "]", joints[i]);
            }

            mesh.draw(position, joints, id);
        }
    }
}

} // namespace Engine
