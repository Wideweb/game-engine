#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "DirectedLight.hpp"
#include "IteratorRange.hpp"
#include "Model.hpp"
#include "ModelInstanceManager.hpp"
#include "Particles.hpp"
#include "Skybox.hpp"
#include "SpotLight.hpp"
#include "Texture.hpp"

namespace Engine {

using ParticlesEmitterInstance = uint32_t;
constexpr ParticlesEmitterInstance NoParticlesEmitterInstance = 0;

constexpr uint32_t c_MaxSceneObjects = 500;
constexpr uint32_t c_MaxSceneLights = 4;
constexpr uint32_t c_MaxParticleEmitters = 10;

struct SceneSpotLight {
    SpotLight light;
    glm::vec3 position;
};

struct SceneParticles {
    Particles particles;
    glm::mat4 position;
};

class Scene {
  public:
    ~Scene() = default;

    using ParticlesRange = IteratorRange<std::array<SceneParticles, c_MaxParticleEmitters>::const_iterator>;

    using ObjectsRange = IteratorRange<std::unordered_map<std::string, ModelInstanceManager>::iterator>;

    using OverlayObjectsRange = IteratorRange<std::unordered_map<std::string, ModelInstanceManager>::iterator>;

    using LightsRange = IteratorRange<std::array<SceneSpotLight, c_MaxSceneLights>::const_iterator>;

    void setSkybox(const std::shared_ptr<Skybox> skybox);
    std::shared_ptr<Skybox> getSkybox();

    ModelInstance addStaticObject(const std::string &model, glm::mat4 position);

    ModelInstance addOverlayObject(const std::string &model, glm::mat4 position);
    void updateOverlayObject(const std::string &model, glm::mat4 position, ModelInstance instance);
    ObjectsRange getOverlayObjects();

    ModelInstance addObject(const std::string &model, glm::mat4 position);
    void updateObject(const std::string &model, glm::mat4 position, ModelInstance instance);
    void updateObject(const std::string &model, std::vector<glm::mat4> joints, ModelInstance instance);
    ObjectsRange getObjects();

    void setDirectedLight(const DirectedLight &light);
    bool hasDirectedLight();
    DirectedLight &getDirectedLight();

    void addSpotLight(const SpotLight &light, glm::vec3 position);
    LightsRange getSpotLights();

    ParticlesEmitterInstance addParticlesEmitter(ParticlesConfiguration config, glm::mat4 position);
    void updateParticlesEmitter(ParticlesEmitterInstance instance, glm::mat4 position, double deltaTime);
    ParticlesRange getParticleEmitters();

    void clear();

  private:
    std::unordered_map<std::string, ModelInstanceManager> m_Objects;
    std::unordered_map<std::string, ModelInstanceManager> m_OverlayObjects;

    DirectedLight m_DirectedLight;
    std::array<SceneSpotLight, c_MaxSceneLights> m_SpotLights;
    std::shared_ptr<Skybox> m_Skybox;

    uint32_t m_ActiveLights = 0;
    bool m_HasDirectedLight = false;

    uint32_t m_ActiveParticleEmitters = 0;
    std::array<SceneParticles, c_MaxParticleEmitters> m_ParticleEmitters;
};

} // namespace Engine
