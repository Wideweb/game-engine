#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
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
#include "Skybox.hpp"
#include "SpotLight.hpp"
#include "Texture.hpp"
#include "Material.hpp"

namespace Engine {

constexpr uint32_t c_MaxSceneLights = 4;

struct SceneDirectedLight {
    DirectedLight light;
    glm::vec3 position;
    glm::quat rotation;
};

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

class Scene {
  public:
    ~Scene() = default;

    void setSkybox(const std::shared_ptr<Skybox> skybox);
    std::shared_ptr<Skybox> getSkybox();

    uint32_t addStaticObject(uint32_t id, const std::string &model, const glm::mat4& position, Material* material);

    uint32_t addObject(uint32_t id, const std::string &model, const glm::mat4& position, Material* material);
    void removeObject(uint32_t id);
    void updateObject(uint32_t id, const std::string &model, const glm::mat4& position, Material* material);
    void updateObject(uint32_t id, const std::vector<glm::mat4>& joints);
    std::vector<Material*>& getMaterials() { return m_Objects.keys(); }
    std::vector<FlatDictionary<std::string, std::shared_ptr<ModelInstanceBatch>>>& getModelsInstances() { return m_Objects.values(); }

    void setDirectedLight(const DirectedLight &light, glm::vec3 position, glm::quat rotation);
    bool hasDirectedLight();
    SceneDirectedLight &getDirectedLight();

    uint32_t addSpotLight(uint32_t id, const SpotLight &light, glm::vec3 position);
    void removeSpotLight(uint32_t id);
    void updateSpotLight(uint32_t id, const SpotLight &light, glm::vec3 position);
    const std::vector<SceneSpotLight> &getSpotLights() const;

    uint32_t addParticlesEmitter(uint32_t id, ParticlesConfiguration config, const glm::mat4& position);
    void removeParticlesEmitter(uint32_t id);
    void updateParticlesEmitter(uint32_t id, const ParticlesConfiguration &config, const glm::mat4& position,
                                double deltaTime);
    const std::vector<SceneParticles> &getParticleEmitters() const;

    uint32_t addText(uint32_t id, std::string text, std::shared_ptr<Font> font, const glm::mat4& transform,
                     const glm::mat4& ndcTransform, glm::vec3 color);
    void removeText(uint32_t id);
    void updateText(uint32_t id, std::string text, std::shared_ptr<Font> font, const glm::mat4& transform,
                    const glm::mat4& ndcTransform, glm::vec3 color);
    const std::vector<SceneText> &getTexts() const;

    void clear();

  private:
    FlatDictionary<Material*, FlatDictionary<std::string, std::shared_ptr<ModelInstanceBatch>>> m_Objects;
    std::unordered_map<uint32_t, Material*> m_IdToMaterial;
    std::unordered_map<uint32_t, std::string> m_IdToModel;

    std::shared_ptr<Skybox> m_Skybox;

    bool m_HasDirectedLight = false;
    SceneDirectedLight m_DirectedLight;

    FlatDictionary<uint32_t, SceneParticles> m_ParticleEmitters;
    FlatDictionary<uint32_t, SceneSpotLight> m_SpotLights;
    FlatDictionary<uint32_t, SceneText> m_Texts;
};

} // namespace Engine
