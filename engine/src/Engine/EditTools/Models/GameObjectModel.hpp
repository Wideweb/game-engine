#pragma once

#include "Application.hpp"
#include "BehaviourComponent.hpp"
#include "CollisionComponent.hpp"
#include "Entity.hpp"
#include "EventDispatcher.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "Material.hpp"
#include "Particles.hpp"
#include "ParticlesComponent.hpp"
#include "Render3DComponent.hpp"
#include "StaticCollisionComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class GameObjectModel {
  private:
    Entity m_Entity = c_NoEntity;

  public:
    EventDispatcher<glm::vec3> positionChange$;
    EventDispatcher<glm::vec3> rotationChange$;
    EventDispatcher<float> scaleChange$;
    EventDispatcher<ParticlesConfiguration> particlesChange$;
    EventDispatcher<float> materialSpecularChange$;
    EventDispatcher<float> materialShininessChange$;

    void setEntity(Entity entity) {
        m_Entity = entity;

        positionChange$.dispatch(position());
        rotationChange$.dispatch(rotation());
        scaleChange$.dispatch(scale());

        if (hasMaterial()) {
            materialSpecularChange$.dispatch(material().specular);
            materialShininessChange$.dispatch(material().shininess);
        }

        if (hasParticles()) {
            particlesChange$.dispatch(particles());
        }
    }

    Entity entity() { return m_Entity; }

    bool isActive() { return m_Entity != c_NoEntity; }

    glm::vec3 position() {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).position;
        }

        return glm::vec3(0.0f);
    }

    void position(glm::vec3 value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        location.position = value;
        location.updated = true;

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(value);
    }

    glm::vec3 rotation() {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).rotation;
        }

        return glm::vec3(0.0f);
    }

    void rotation(glm::vec3 value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        location.rotation = value;
        location.updated = true;

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        rotationChange$.dispatch(value);
    }

    float scale() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return coordinator().GetComponent<Render3DComponent>(m_Entity).scale;
        }

        return 0.0f;
    }

    void scale(float value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            auto &collision = coordinator().GetComponent<StaticCollisionComponent>(m_Entity);
            std::vector<glm::vec3> vertices;
            vertices.reserve(8);
            std::transform(collision.vertices.begin(), collision.vertices.end(), std::back_inserter(vertices),
                           [&](const glm::vec3 &v) { return v / render.scale * value; });
            collision.vertices = std::move(vertices);
            collision.updated = true;
        }

        render.scale = value;

        render.updated = true;
        location.updated = true;

        scaleChange$.dispatch(value);
    }

    bool hasParticles() { return coordinator().HasComponent<ParticlesComponent>(m_Entity); }

    ParticlesConfiguration particles() {
        if (hasParticles()) {
            return coordinator().GetComponent<ParticlesComponent>(m_Entity).emitter;
        }

        return {};
    }

    void particles(ParticlesConfiguration value) {
        if (hasParticles()) {
            coordinator().GetComponent<ParticlesComponent>(m_Entity).emitter = value;
            particlesChange$.dispatch(value);
        }
    }

    void setTexture(std::shared_ptr<Texture> texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.diffuseMap = texture;
            mesh.material.specularMap = texture;
            mesh.material.normalMap = texture;
            mesh.hasMaterial = true;
        }
    }

    void materialSpecular(float value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.specular = value;
        }

        materialSpecularChange$.dispatch(value);
    }

    void materialShininess(float value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.shininess = value;
        }

        materialShininessChange$.dispatch(value);
    }

    const Material &material() {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        return model->meshes[0].material;
    }

    bool hasMaterial() { return coordinator().HasComponent<Render3DComponent>(m_Entity); }

    void behaviour(std::string script) {
        if (coordinator().HasComponent<BehaviourComponent>(m_Entity)) {
            layer().getScripts().remove(m_Entity);
            auto &behaviour = coordinator().GetComponent<BehaviourComponent>(m_Entity);
            behaviour.script = script;
            behaviour.invalid = false;
        } else {
            coordinator().AddComponent<BehaviourComponent>(m_Entity, BehaviourComponent(script));
        }
    }

  private:
    Layer &layer() { return Application::get().getLayer("game_area"); }
    Coordinator &coordinator() { return layer().getCoordinator(); }
};

} // namespace Engine
