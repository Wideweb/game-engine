#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IteratorRange.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "ModelInstanceManager.hpp"
#include "Skybox.hpp"
#include "Texture.hpp"

namespace Engine {

constexpr uint32_t c_MaxSceneObjects = 500;
constexpr uint32_t c_MaxSceneLights = 4;

struct SceneLight {
    Light light;
    glm::vec3 position;
};

class Scene {
  public:
    ~Scene() = default;

    using SceneObjectsRange = IteratorRange<
        std::unordered_map<std::string, ModelInstanceManager>::iterator>;

    using SceneLightsRange =
        IteratorRange<std::array<SceneLight, c_MaxSceneLights>::const_iterator>;

    void setSkybox(const std::shared_ptr<Skybox> skybox);
    std::shared_ptr<Skybox> getSkybox();

    ModelInstance addObject(const std::string &model, glm::mat4 position);
    void updateObject(const std::string &model, glm::mat4 position,
                      ModelInstance instance);
    void addLight(const Light &light, glm::vec3 position);

    SceneObjectsRange getObjects();
    SceneLightsRange getLights();

    void clear();

  private:
    std::unordered_map<std::string, ModelInstanceManager> m_Objects;

    std::array<SceneLight, c_MaxSceneLights> m_Lights;
    std::shared_ptr<Skybox> m_Skybox;

    uint32_t m_ActiveLights = 0;
};

} // namespace Engine
