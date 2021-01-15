#include "Scene.hpp"

namespace Engine {

void Scene::setSkybox(const std::shared_ptr<Skybox> skybox) {
    m_Skybox = skybox;
}

std::shared_ptr<Skybox> Scene::getSkybox() { return m_Skybox; }

ModelInstance Scene::addObject(const std::string &model, glm::mat4 position) {
    return m_Objects[model].Create(std::move(position));
}

void Scene::updateObject(const std::string &model, glm::mat4 position,
                         ModelInstance instance) {
    m_Objects[model].Update(instance, std::move(position));
}

void Scene::addLight(const Light &light, glm::vec3 position) {
    m_Lights[m_ActiveLights].light = light;
    m_Lights[m_ActiveLights].position = std::move(position);
    ++m_ActiveLights;
}

Scene::SceneObjectsRange Scene::getObjects() {
    return {m_Objects.begin(), m_Objects.end()};
}

Scene::SceneLightsRange Scene::getLights() {
    return {m_Lights.cbegin(), m_Lights.cbegin() + m_ActiveLights};
}

void Scene::clear() { m_ActiveLights = 0; }

} // namespace Engine
