#include "CubeMap.hpp"

#include <glm/gtx/transform.hpp>

#include "glad/glad.h"

namespace Engine {

CubeMap::CubeMap() {}

CubeMap::CubeMap(int width, int height, float nearPlane, float farPlane, glm::vec3 position)
    : m_FarPlane(farPlane), m_Position(position) {
    m_CubeMapTexture = CubeMapTexture::createCubeDepthBuffer(width, height);
    set(width, height, nearPlane, farPlane);
}

void CubeMap::free() {
    if (!m_CubeMapTexture.empty()) {
        m_CubeMapTexture.free();
    }
}

void CubeMap::bind(Shader &shader) {
    shader.setFloat("u_farPlane", m_FarPlane);
    shader.setFloat3("u_cubePosition", m_Position);
    for (unsigned int i = 0; i < 6; ++i) {
        shader.setMatrix4("u_cubeMatrices[" + std::to_string(i) + "]", m_Transforms[i]);
    }
}

void CubeMap::set(int width, int height, float nearPlane, float farPlane) {
    if (m_CubeMapTexture.width != width || m_CubeMapTexture.height != height) {
        m_CubeMapTexture.resize(width, height);
    }
    float aspect = width / height;

    m_Projection = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

    updateTransforms();
}

void CubeMap::setPosition(const glm::vec3 &position) {
    m_Position = position;
    updateTransforms();
}

void CubeMap::updateTransforms() {
    m_Transforms[0] =
        m_Projection * glm::lookAt(m_Position, m_Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    m_Transforms[1] =
        m_Projection * glm::lookAt(m_Position, m_Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    m_Transforms[2] =
        m_Projection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    m_Transforms[3] =
        m_Projection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
    m_Transforms[4] =
        m_Projection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    m_Transforms[5] =
        m_Projection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
}

} // namespace Engine
