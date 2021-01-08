#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "File.hpp"
#include "Render.hpp"

namespace Engine {

Render::Render() {
    auto vertexSrc = File::read("./shaders/vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/fragment-shader.glsl");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/skybox-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/skybox-fragment-shader.glsl");
    m_SkyboxShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
}

void Render::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Render::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Render::draw(const Scene &scene, const Camera &camera) {
    m_Shader->bind();

    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = camera.projectionMatrix();

    m_Shader->setMatrix4("View", glm::value_ptr(view));
    m_Shader->setMatrix4("Projection", glm::value_ptr(projection));
    m_Shader->setFloat3("ViewPos", camera.positionVec());

    for (const auto &object : scene.getLights()) {
        m_Shader->setFloat3("Light.position", object.position);
        m_Shader->setFloat3("Light.ambient", object.light.ambient);
        m_Shader->setFloat3("Light.diffuse", object.light.diffuse);
        m_Shader->setFloat3("Light.specular", object.light.specular);
        m_Shader->setFloat("Light.constant", object.light.constant);
        m_Shader->setFloat("Light.linear", object.light.linear);
        m_Shader->setFloat("Light.quadratic", object.light.quadratic);
    }

    for (const auto &object : scene.getObjects()) {
        for (const auto &mesh : object.model->meshes) {
            m_Shader->setMatrix4("Model", glm::value_ptr(object.position));
            mesh.draw(*m_Shader);
        }
    }

    if (scene.getSkybox()) {
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        m_SkyboxShader->bind();

        auto fixedView = glm::mat4(glm::mat3(view));

        m_SkyboxShader->setMatrix4("View", glm::value_ptr(fixedView));
        m_SkyboxShader->setMatrix4("Projection", glm::value_ptr(projection));

        scene.getSkybox()->draw(*m_SkyboxShader);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
}

} // namespace Engine
