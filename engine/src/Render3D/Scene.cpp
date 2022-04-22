#include "Scene.hpp"

#include <utility>

namespace Engine {

void Scene::setSkybox(const std::shared_ptr<Skybox> skybox) { m_Skybox = skybox; }

std::shared_ptr<Skybox> Scene::getSkybox() { return m_Skybox; }

uint32_t Scene::addOverlayObject(uint32_t id, const std::string &model, glm::mat4 position,
                                 std::shared_ptr<Shader> shader) {
    m_OverlayObjects[model].create(id, std::move(position), false);
    m_OverlayObjects[model].update(id, shader);
    return id;
}

void Scene::removeOverlayObject(uint32_t id, const std::string &model) { m_OverlayObjects[model].remove(id); }

void Scene::updateOverlayObject(uint32_t id, const std::string &model, glm::mat4 position,
                                std::shared_ptr<Shader> shader) {
    m_OverlayObjects[model].update(id, std::move(position));
    m_OverlayObjects[model].update(id, shader);
}

Scene::ObjectsRange Scene::getOverlayObjects() { return {m_OverlayObjects.begin(), m_OverlayObjects.end()}; }

uint32_t Scene::addStaticObject(uint32_t id, const std::string &model, glm::mat4 position) {
    m_Objects[model].create(id, std::move(position), true);
    return id;
}

uint32_t Scene::addObject(uint32_t id, const std::string &model, glm::mat4 position, std::shared_ptr<Shader> shader) {
    m_Objects[model].create(id, std::move(position), false);
    m_Objects[model].update(id, shader);
    return id;
}

void Scene::removeObject(uint32_t id, const std::string &model) { m_Objects[model].remove(id); }

void Scene::updateObject(uint32_t id, const std::string &model, glm::mat4 position, std::shared_ptr<Shader> shader) {
    m_Objects[model].update(id, std::move(position));
    m_Objects[model].update(id, shader);
}

void Scene::updateObject(uint32_t id, const std::string &model, std::vector<glm::mat4> joints) {
    m_Objects[model].update(id, std::move(joints));
}

void Scene::updateObject(uint32_t id, const std::string &model, std::shared_ptr<Shader> shader) {
    m_Objects[model].update(id, shader);
}

Scene::ObjectsRange Scene::getObjects() { return {m_Objects.begin(), m_Objects.end()}; }

void Scene::setDirectedLight(const DirectedLight &light) {
    m_DirectedLight = light;
    m_HasDirectedLight = true;
}

bool Scene::hasDirectedLight() { return m_HasDirectedLight; }

DirectedLight &Scene::getDirectedLight() { return m_DirectedLight; }

uint32_t Scene::addSpotLight(uint32_t id, const SpotLight &light, glm::vec3 position) {
    m_SpotLights.emplace(id);
    auto &instance = m_SpotLights[id];
    instance.light = light;
    instance.position = position;
    return id;
}

void Scene::removeSpotLight(uint32_t id) { m_SpotLights.remove(id); }

void Scene::updateSpotLight(uint32_t id, const SpotLight &light, glm::vec3 position) {
    auto &instance = m_SpotLights[id];
    instance.light = light;
    instance.position = position;
}

const std::vector<SceneSpotLight> &Scene::getSpotLights() const { return m_SpotLights.values(); }

uint32_t Scene::addParticlesEmitter(uint32_t id, ParticlesConfiguration config, glm::mat4 position) {
    m_ParticleEmitters.emplace(id);
    auto &instance = m_ParticleEmitters[id];
    instance.particles.setUp(config);
    instance.position = position;
    return id;
}

void Scene::removeParticlesEmitter(uint32_t id) {
    auto &instance = m_ParticleEmitters[id];
    instance.particles.clear();
    m_ParticleEmitters.remove(id);
}

void Scene::updateParticlesEmitter(uint32_t id, const ParticlesConfiguration &config, glm::mat4 position,
                                   double deltaTime) {
    auto &instance = m_ParticleEmitters[id];

    instance.position = position;

    if (instance.particles.config() != config) {
        instance.particles.setUp(config);
    }

    instance.particles.update(deltaTime);
}

const std::vector<SceneParticles> &Scene::getParticleEmitters() const { return m_ParticleEmitters.values(); }

uint32_t Scene::addText(uint32_t id, std::string text, std::shared_ptr<Font> font, glm::mat4 transform,
                        glm::mat4 ndcTransform, glm::vec3 color) {
    m_Texts.add(id, {id, text, font, transform, ndcTransform, color});
    return id;
}

void Scene::removeText(uint32_t id) { m_Texts.remove(id); }

void Scene::updateText(uint32_t id, std::string text, std::shared_ptr<Font> font, glm::mat4 transform,
                       glm::mat4 ndcTransform, glm::vec3 color) {
    auto &instance = m_Texts[id];
    instance.text = text;
    instance.font = font;
    instance.transform = transform;
    instance.ndcTransform = ndcTransform;
    instance.color = color;
}

const std::vector<SceneText> &Scene::getTexts() const { return m_Texts.values(); }

void Scene::clear() { m_HasDirectedLight = false; }

} // namespace Engine
