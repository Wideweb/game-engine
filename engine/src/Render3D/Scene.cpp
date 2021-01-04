#include "Scene.hpp"

namespace Engine {

void Scene::addObject(const std::shared_ptr<Model> &model,
                      const glm::mat4 &position) {
    m_Objects[m_ActiveObjects].model = model;
    m_Objects[m_ActiveObjects].position = position;
    ++m_ActiveObjects;
}

void Scene::addLight(const Light &light, const glm::vec3 &position) {
    m_Lights[m_ActiveLights].light = light;
    m_Lights[m_ActiveLights].position = position;
    ++m_ActiveLights;
}

const Scene::SceneObjectsRange Scene::getObjects() const {
    return {m_Objects.cbegin(), m_Objects.cbegin() + m_ActiveObjects};
}

const Scene::SceneLightsRange Scene::getLights() const {
    return {m_Lights.cbegin(), m_Lights.cbegin() + m_ActiveLights};
}

void Scene::clear() {
    m_ActiveObjects = 0;
    m_ActiveLights = 0;
}

} // namespace Engine
