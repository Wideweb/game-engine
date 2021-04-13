#include "CubeMap.hpp"

#include "TextureLoader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "glad/glad.h"

namespace Engine {

CubeMap::CubeMap(int width, int height, float farPlane, glm::vec3 position)
    : m_FarPlane(farPlane), m_Position(position) {
    m_CubeMapTexture.reset(TextureLoader::createCubeDepthBuffer(width, height));

    float aspect = width / height;
    float near = 1.0f;

    m_Projection =
        glm::perspective(glm::radians(90.0f), aspect, near, farPlane);

    updateTransforms();
}

CubeMap::~CubeMap() {}

void CubeMap::bind(Shader &shader) {
    glActiveTexture(GL_TEXTURE0);
    m_CubeMapTexture->bind();

    shader.setFloat("FarPlane", m_FarPlane);
    shader.setFloat3("CubePosition", m_Position);
    for (unsigned int i = 0; i < 6; ++i) {
        shader.setMatrix4("CubeMatrices[" + std::to_string(i) + "]",
                          glm::value_ptr(m_Transforms[i]));
    }
}

void CubeMap::setPosition(const glm::vec3 &position) {
    m_Position = position;
    updateTransforms();
}

void CubeMap::updateTransforms() {
    m_Transforms[0] =
        m_Projection * glm::lookAt(m_Position,
                                   m_Position + glm::vec3(1.0, 0.0, 0.0),
                                   glm::vec3(0.0, -1.0, 0.0));
    m_Transforms[1] =
        m_Projection * glm::lookAt(m_Position,
                                   m_Position + glm::vec3(-1.0, 0.0, 0.0),
                                   glm::vec3(0.0, -1.0, 0.0));
    m_Transforms[2] =
        m_Projection * glm::lookAt(m_Position,
                                   m_Position + glm::vec3(0.0, 1.0, 0.0),
                                   glm::vec3(0.0, 0.0, 1.0));
    m_Transforms[3] =
        m_Projection * glm::lookAt(m_Position,
                                   m_Position + glm::vec3(0.0, -1.0, 0.0),
                                   glm::vec3(0.0, 0.0, -1.0));
    m_Transforms[4] =
        m_Projection * glm::lookAt(m_Position,
                                   m_Position + glm::vec3(0.0, 0.0, 1.0),
                                   glm::vec3(0.0, -1.0, 0.0));
    m_Transforms[5] =
        m_Projection * glm::lookAt(m_Position,
                                   m_Position + glm::vec3(0.0, 0.0, -1.0),
                                   glm::vec3(0.0, -1.0, 0.0));
}

} // namespace Engine
