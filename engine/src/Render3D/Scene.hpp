#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>

#include "IteratorRange.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Skybox.hpp"
#include "Texture.hpp"

namespace Engine {

constexpr uint32_t c_MaxSceneObjects = 500;
constexpr uint32_t c_MaxSceneLights = 4;

struct SceneObject {
    std::shared_ptr<Model> model;
    glm::mat4 position;
};

struct SceneLight {
    Light light;
    glm::vec3 position;
};

class Scene {
  public:
    ~Scene() = default;

    using SceneObjectsRange = IteratorRange<
        std::array<SceneObject, c_MaxSceneObjects>::const_iterator>;

    using SceneLightsRange =
        IteratorRange<std::array<SceneLight, c_MaxSceneLights>::const_iterator>;

    void setSkybox(const std::shared_ptr<Skybox> skybox);
    const std::shared_ptr<Skybox> getSkybox() const;

    void addObject(const std::shared_ptr<Model> &model, glm::mat4 position);
    void addLight(const Light &light, glm::vec3 position);

    const SceneObjectsRange getObjects() const;
    const SceneLightsRange getLights() const;

    void clear();

  private:
    std::array<SceneObject, c_MaxSceneObjects> m_Objects;
    std::array<SceneLight, c_MaxSceneLights> m_Lights;
    std::shared_ptr<Skybox> m_Skybox;

    uint32_t m_ActiveObjects = 0;
    uint32_t m_ActiveLights = 0;
};

} // namespace Engine
