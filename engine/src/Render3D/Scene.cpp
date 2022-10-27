#include "Scene.hpp"

#include <utility>

namespace Engine {

void Scene::setSkybox(const std::shared_ptr<Skybox> skybox) { m_Skybox = skybox; }

std::shared_ptr<Skybox> Scene::getSkybox() { return m_Skybox; }

uint32_t Scene::addStaticObject(uint32_t id, const std::string &model, const glm::mat4& position, Material* material) {
    return addObject(id, model, position, material);
}

uint32_t Scene::addObject(uint32_t id, const std::string &model, const glm::mat4& position, Material* material) {
    if (!m_Objects.hasKey(material)) {
        m_Objects.emplace(material);
    }

    if (!m_Objects[material].hasKey(model)) {
        m_Objects[material].emplace(model, std::make_shared<ModelInstanceBatch>());
    }

    m_Objects[material][model]->create(id, position);

    m_IdToMaterial[id] = material;
    m_IdToModel[id] = model;

    return id;
}

void Scene::removeObject(uint32_t id) {
    Material* material = m_IdToMaterial[id];
    const std::string& model = m_IdToModel[id];

    m_Objects[material][model]->remove(id);
    if (m_Objects[material][model]->empty()) {
        m_Objects[material].remove(model);

        if (m_Objects[material].empty()) {
            m_Objects.remove(material);
        }
    }

    m_IdToMaterial.erase(id);
    m_IdToModel.erase(id);
}

void Scene::updateObject(uint32_t id, const std::string &model, const glm::mat4& position, Material* material) {
    removeObject(id);
    addObject(id, model, position, material);
}

void Scene::updateObject(uint32_t id, const std::vector<glm::mat4>& joints) {
    Material* material = m_IdToMaterial[id];
    const std::string& model = m_IdToModel[id];

    m_Objects[material][model]->update(id, joints);
}

void Scene::setDirectedLight(const DirectedLight &light, glm::vec3 position, glm::quat rotation) {
    m_DirectedLight = {light, position, rotation};
    m_HasDirectedLight = true;
}

bool Scene::hasDirectedLight() { return m_HasDirectedLight; }

SceneDirectedLight &Scene::getDirectedLight() { return m_DirectedLight; }

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

uint32_t Scene::addParticlesEmitter(uint32_t id, ParticlesConfiguration config, const glm::mat4& position) {
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

void Scene::updateParticlesEmitter(uint32_t id, const ParticlesConfiguration &config, const glm::mat4& position,
                                   double deltaTime) {
    auto &instance = m_ParticleEmitters[id];

    instance.position = position;

    if (instance.particles.config() != config) {
        instance.particles.setUp(config);
    }

    instance.particles.update(deltaTime);
}

const std::vector<SceneParticles> &Scene::getParticleEmitters() const { return m_ParticleEmitters.values(); }

uint32_t Scene::addText(uint32_t id, std::string text, std::shared_ptr<Font> font, const glm::mat4& transform,
                        const glm::mat4& ndcTransform, glm::vec3 color) {
    m_Texts.add(id, {id, text, font, transform, ndcTransform, color});
    return id;
}

void Scene::removeText(uint32_t id) { m_Texts.remove(id); }

void Scene::updateText(uint32_t id, std::string text, std::shared_ptr<Font> font, const glm::mat4& transform,
                       const glm::mat4& ndcTransform, glm::vec3 color) {
    auto& instance = m_Texts[id];
    instance.text = text;
    instance.font = font;
    instance.transform = transform;
    instance.ndcTransform = ndcTransform;
    instance.color = color;
}

const std::vector<SceneText> &Scene::getTexts() const { return m_Texts.values(); }

void Scene::clear() { m_HasDirectedLight = false; }

} // namespace Engine
