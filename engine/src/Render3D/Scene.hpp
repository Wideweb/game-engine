#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "DirectedLight.hpp"
#include "FlatDictionary.hpp"
#include "Font.hpp"
#include "IteratorRange.hpp"
#include "Model.hpp"
#include "Particles.hpp"
#include "ShaderModelInstanceManager.hpp"
#include "Skybox.hpp"
#include "SpotLight.hpp"
#include "Texture.hpp"

namespace Engine {

constexpr uint32_t c_MaxSceneLights = 4;

struct SceneSpotLight {
    SpotLight light;
    glm::vec3 position;
};

struct SceneText {
    uint32_t id;
    std::string text;
    std::shared_ptr<Font> font;
    glm::mat4 transform;
    glm::mat4 ndcTransform;
    glm::vec3 color;
};

struct SceneParticles {
    Particles particles;
    glm::mat4 position;
};

struct SceneShaderObject {
    std::string model;
    glm::mat4 position;
    std::shared_ptr<Shader> shader;
};

class Scene {
  public:
    ~Scene() = default;

    using ObjectsRange = IteratorRange<std::unordered_map<std::string, ShaderModelInstanceManager>::iterator>;

    void setSkybox(const std::shared_ptr<Skybox> skybox);
    std::shared_ptr<Skybox> getSkybox();

    uint32_t addStaticObject(uint32_t id, const std::string &model, glm::mat4 position);

    uint32_t addOverlayObject(uint32_t id, const std::string &model, glm::mat4 position,
                              std::shared_ptr<Shader> shader = nullptr);
    void removeOverlayObject(uint32_t id, const std::string &model);
    void updateOverlayObject(uint32_t id, const std::string &model, glm::mat4 position,
                             std::shared_ptr<Shader> shader = nullptr);
    ObjectsRange getOverlayObjects();

    uint32_t addObject(uint32_t id, const std::string &model, glm::mat4 position,
                       std::shared_ptr<Shader> shader = nullptr);
    void removeObject(uint32_t id, const std::string &model);
    void updateObject(uint32_t id, const std::string &model, glm::mat4 position, std::shared_ptr<Shader> shader);
    void updateObject(uint32_t id, const std::string &model, std::vector<glm::mat4> joints);
    void updateObject(uint32_t id, const std::string &model, std::shared_ptr<Shader> shader = nullptr);
    ObjectsRange getObjects();

    void setDirectedLight(const DirectedLight &light);
    bool hasDirectedLight();
    DirectedLight &getDirectedLight();

    uint32_t addSpotLight(uint32_t id, const SpotLight &light, glm::vec3 position);
    void removeSpotLight(uint32_t id);
    void updateSpotLight(uint32_t id, const SpotLight &light, glm::vec3 position);
    const std::vector<SceneSpotLight> &getSpotLights() const;

    uint32_t addParticlesEmitter(uint32_t id, ParticlesConfiguration config, glm::mat4 position);
    void removeParticlesEmitter(uint32_t id);
    void updateParticlesEmitter(uint32_t id, const ParticlesConfiguration &config, glm::mat4 position,
                                double deltaTime);
    const std::vector<SceneParticles> &getParticleEmitters() const;

    uint32_t addText(uint32_t id, std::string text, std::shared_ptr<Font> font, glm::mat4 transform,  glm::mat4 ndcTransform, glm::vec3 color);
    void removeText(uint32_t id);
    void updateText(uint32_t id, std::string text, std::shared_ptr<Font> font, glm::mat4 transform, glm::mat4 ndcTransform, glm::vec3 color);
    const std::vector<SceneText> &getTexts() const;

    void clear();

  private:
    std::unordered_map<std::string, ShaderModelInstanceManager> m_Objects;
    std::unordered_map<std::string, ShaderModelInstanceManager> m_OverlayObjects;

    std::shared_ptr<Skybox> m_Skybox;

    bool m_HasDirectedLight = false;
    DirectedLight m_DirectedLight;

    FlatDictionary<uint32_t, SceneParticles> m_ParticleEmitters;
    FlatDictionary<uint32_t, SceneSpotLight> m_SpotLights;
    FlatDictionary<uint32_t, SceneText> m_Texts;
};

} // namespace Engine
