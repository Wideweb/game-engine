#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "File.hpp"
#include "Render.hpp"

namespace Engine {

Render::Render() {}

void Render::init() {
    auto vertexSrc = File::read("./shaders/vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/fragment-shader.glsl");
    shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
}

void Render::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Render::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Render::draw(const Scene &scene, const Camera &camera) {
    shader->bind();

    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = camera.projectionMatrix();

    shader->setMatrix4("View", glm::value_ptr(view));
    shader->setMatrix4("Projection", glm::value_ptr(projection));
    shader->setFloat3("ViewPos", camera.positionVec());

    for (const auto &object : scene.getLights()) {
        shader->setFloat3("Light.position", object.position);
        shader->setFloat3("Light.ambient", object.light.ambient);
        shader->setFloat3("Light.diffuse", object.light.diffuse);
        shader->setFloat3("Light.specular", object.light.specular);
        shader->setFloat("Light.constant", object.light.constant);
        shader->setFloat("Light.linear", object.light.linear);
        shader->setFloat("Light.quadratic", object.light.quadratic);
    }

    for (const auto &object : scene.getObjects()) {
        for (const auto &mesh : object.model->meshes) {
            shader->setMatrix4("Model", glm::value_ptr(object.position));
            mesh.draw(*shader);
        }
    }
}

} // namespace Engine
