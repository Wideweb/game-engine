#include "InstancedModel.hpp"

#include "glad/glad.h"

namespace Engine {

InstancedModel::InstancedModel() {}

InstancedModel::InstancedModel(const std::vector<Mesh> &meshes) : meshes(meshes) {}

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

void InstancedModel::draw(Shader &shader, ShaderModelInstanceManager &instances) {
    for (const auto &it : instances.shaders()) {
        ShaderId shaderId = it ? it->id : c_NoShader;

        GLuint instanceVBO, idVBO;
        if (shaderToInstanceVBO.find(shaderId) == shaderToInstanceVBO.end()) {
            glGenBuffers(1, &idVBO);
            glGenBuffers(1, &instanceVBO);

            shaderToInstanceVBO[shaderId] = instanceVBO;
            shaderToIdVBO[shaderId] = idVBO;
        } else {
            instanceVBO = shaderToInstanceVBO[shaderId];
            idVBO = shaderToIdVBO[shaderId];
        }

        auto &shaderInstances = instances.instances(shaderId);
        if (shaderInstances.resized) {
            setInstances(instanceVBO, idVBO, shaderInstances.positions(), shaderInstances.ids());
        } else if (shaderInstances.updated) {
            updateInstances(instanceVBO, idVBO, shaderInstances.from, shaderInstances.to, shaderInstances.positions(),
                            shaderInstances.ids());
        }

        for (const auto &mesh : meshes) {
            mesh.setInstances(idVBO, instanceVBO);

            shaderInstances.resized = false;
            shaderInstances.updated = false;

            mesh.draw(it ? *it : shader, shaderInstances.size());
        }
    }

    // for (const auto &mesh : meshes) {
    //     auto &shaderInstances = instances.instances(c_NoShader);
    //     if (shaderInstances.resized) {
    //         setInstances(shaderInstances.positions(), shaderInstances.ids());
    //     } else if (shaderInstances.updated) {
    //         updateInstances(shaderInstances.from, shaderInstances.to, shaderInstances.positions(),
    //                         shaderInstances.ids());
    //     }

    // auto size = static_cast<GLint>(sizeof(uint32_t) * instances.size());
    // glBindBuffer(GL_COPY_READ_BUFFER, idVBO);
    // glBindBuffer(GL_COPY_WRITE_BUFFER, mesh.idVBO);
    // glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
    // glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    // size = static_cast<GLint>(sizeof(glm::mat4) * instances.size());
    // glBindBuffer(GL_COPY_READ_BUFFER, instanceVBO);
    // glBindBuffer(GL_COPY_WRITE_BUFFER, mesh.instanceVBO);
    // glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
    // glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    // glBindBuffer(GL_COPY_READ_BUFFER, 0);
    // glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

    // glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
    // }
}

void InstancedModel::setInstances(GLuint instanceVBO, GLuint idVBO, const std::vector<glm::mat4> &positions,
                                  const std::vector<uint32_t> &ids) const {
    auto size = static_cast<GLint>(sizeof(glm::mat4) * positions.size());
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, size, positions.data(), GL_STATIC_DRAW);

    size = static_cast<GLint>(sizeof(uint32_t) * ids.size());
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferData(GL_ARRAY_BUFFER, size, ids.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedModel::updateInstances(GLuint instanceVBO, GLuint idVBO, size_t from, size_t to,
                                     const std::vector<glm::mat4> &positions, const std::vector<uint32_t> &ids) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::mat4) * from),
                    static_cast<GLsizeiptr>(sizeof(glm::mat4) * (to - from + 1)), positions.data() + from);

    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * from),
                    static_cast<GLsizeiptr>(sizeof(uint32_t) * (to - from + 1)), ids.data() + from);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace Engine
