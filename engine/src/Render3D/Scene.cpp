#include "Scene.hpp"

#include <utility>

namespace Engine {

void Scene::setSkybox(const std::shared_ptr<Skybox> skybox) { m_Skybox = skybox; }

std::shared_ptr<Skybox> Scene::getSkybox() { return m_Skybox; }

ModelInstance Scene::addOverlayObject(const std::string &model, glm::mat4 position, uint32_t id) {
    return m_OverlayObjects[model].Create(std::move(position), false, id);
}

void Scene::removeOverlayObject(const std::string &model, ModelInstance instance) {
    return m_OverlayObjects[model].Remove(instance);
}

void Scene::updateOverlayObject(const std::string &model, glm::mat4 position, ModelInstance instance) {
    m_OverlayObjects[model].Update(instance, std::move(position));
}

Scene::ObjectsRange Scene::getOverlayObjects() { return {m_OverlayObjects.begin(), m_OverlayObjects.end()}; }

ModelInstance Scene::addStaticObject(const std::string &model, glm::mat4 position, uint32_t id) {
    return m_Objects[model].Create(std::move(position), true, id);
}

ModelInstance Scene::addObject(const std::string &model, glm::mat4 position, uint32_t id) {
    return m_Objects[model].Create(std::move(position), false, id);
}

void Scene::removeObject(const std::string &model, ModelInstance instance) { return m_Objects[model].Remove(instance); }

void Scene::updateObject(const std::string &model, glm::mat4 position, ModelInstance instance) {
    m_Objects[model].Update(instance, std::move(position));
}

void Scene::updateObject(const std::string &model, std::vector<glm::mat4> joints, ModelInstance instance) {
    m_Objects[model].Update(instance, std::move(joints));
}

void Scene::setDirectedLight(const DirectedLight &light) {
    m_DirectedLight = light;
    m_HasDirectedLight = true;
}

bool Scene::hasDirectedLight() { return m_HasDirectedLight; }

DirectedLight &Scene::getDirectedLight() { return m_DirectedLight; }

void Scene::addSpotLight(const SpotLight &light, glm::vec3 position) {
    m_SpotLights[m_ActiveLights].light = light;
    m_SpotLights[m_ActiveLights].position = std::move(position);
    ++m_ActiveLights;
}

Scene::ObjectsRange Scene::getObjects() { return {m_Objects.begin(), m_Objects.end()}; }

Scene::LightsRange Scene::getSpotLights() { return {m_SpotLights.cbegin(), m_SpotLights.cbegin() + m_ActiveLights}; }

ParticlesEmitterInstance Scene::addParticlesEmitter(ParticlesConfiguration config, glm::mat4 position) {
    m_ParticleEmitters[m_ActiveParticleEmitters].particles.setUp(config);
    m_ParticleEmitters[m_ActiveParticleEmitters].position = position;
    ++m_ActiveParticleEmitters;
    return m_ActiveParticleEmitters;
}

void Scene::removeParticlesEmitter(ModelInstance instance) {
    size_t index = instance - 1;

    m_ParticleEmitters[index].particles.clear();
    std::swap(m_ParticleEmitters[index], m_ParticleEmitters[m_ActiveParticleEmitters - 1]);

    --m_ActiveParticleEmitters;
}

void Scene::updateParticlesEmitter(ParticlesEmitterInstance instance, const ParticlesConfiguration &config,
                                   glm::mat4 position, double deltaTime) {
    auto &emitter = m_ParticleEmitters[instance - 1];
    emitter.position = position;

    if (emitter.particles.config() != config) {
        emitter.particles.setUp(config);
    }

    emitter.particles.update(deltaTime);
}

Scene::ParticlesRange Scene::getParticleEmitters() {
    return {m_ParticleEmitters.cbegin(), m_ParticleEmitters.cbegin() + m_ActiveParticleEmitters};
}

void Scene::clear() {
    m_ActiveLights = 0;
    m_HasDirectedLight = false;
}

} // namespace Engine
