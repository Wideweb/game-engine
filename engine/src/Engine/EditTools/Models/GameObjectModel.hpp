#pragma once

#include "Application.hpp"
#include "BehaviourComponent.hpp"
#include "CollisionComponent.hpp"
#include "DirectedLightComponent.hpp"
#include "Entity.hpp"
#include "EventDispatcher.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "Material.hpp"
#include "Particles.hpp"
#include "ParticlesComponent.hpp"
#include "Render3DComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "VelocityComponent.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class GameObjectModel {
  private:
    Entity m_Entity = c_NoEntity;

  public:
    EventDispatcher<glm::vec3> positionChange$;
    EventDispatcher<glm::vec3> rotationChange$;
    EventDispatcher<glm::vec3> localRotationChange$;
    EventDispatcher<float, float> scaleChange$;
    EventDispatcher<ParticlesConfiguration> particlesChange$;
    EventDispatcher<float> materialSpecularChange$;
    EventDispatcher<float> materialShininessChange$;
    EventDispatcher<glm::vec3> velocityChange$;
    EventDispatcher<glm::vec3> rotationVelocityChange$;
    EventDispatcher<float> directionalSpeedChange$;

    void setEntity(Entity entity) {
        m_Entity = entity;

        positionChange$.dispatch(position());
        rotationChange$.dispatch(rotation());
        localRotationChange$.dispatch(localRotation());
        scaleChange$.dispatch(scale(), scale());

        if (hasMaterial()) {
            materialSpecularChange$.dispatch(material().specular);
            materialShininessChange$.dispatch(material().shininess);
        }

        if (hasParticles()) {
            particlesChange$.dispatch(particles());
        }

        if (hasVelocity()) {
            velocityChange$.dispatch(velocity());
            rotationVelocityChange$.dispatch(rotationVelocity());
            directionalSpeedChange$.dispatch(directionalSpeed());
        }
    }

    Entity entity() { return m_Entity; }

    bool isActive() { return m_Entity != c_NoEntity; }

    bool hasPosition() {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return true;
        }

        return false;
    }

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

    bool hasRotation() {
        if (coordinator().HasComponent<DirectedLightComponent>(m_Entity)) {
            return true;
        }

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return true;
        }

        return false;
    }

    glm::vec3 rotation() {
        if (coordinator().HasComponent<DirectedLightComponent>(m_Entity)) {
            return coordinator().GetComponent<DirectedLightComponent>(m_Entity).light.direction;
        }

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).rotation;
        }

        return glm::vec3(0.0f);
    }

    void rotation(glm::vec3 value) {
        if (coordinator().HasComponent<DirectedLightComponent>(m_Entity)) {
            auto &light = coordinator().GetComponent<DirectedLightComponent>(m_Entity).light;
            light.direction = value;
        }

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

            location.rotation = value;
            location.updated = true;

            if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
                coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
            }
        }

        rotationChange$.dispatch(value);
    }

    glm::vec3 localRotation() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return coordinator().GetComponent<Render3DComponent>(m_Entity).rotation;
        }

        return glm::vec3(0.0f);
    }

    void localRotation(glm::vec3 value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        render.rotation = value;
        render.updated = true;
        localRotationChange$.dispatch(value);
    }

    bool hasScale() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return true;
        }

        return false;
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

        float oldValue = render.scale;
        render.scale = value;

        render.updated = true;
        location.updated = true;

        scaleChange$.dispatch(value, oldValue);
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

    void setDiffuseMap(std::shared_ptr<Texture> texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.diffuseMap = texture;

            if (mesh.material.specularMap == nullptr) {
                mesh.material.specularMap = Application::get().getTextures().get("engine_placeholder");
            }

            if (mesh.material.normalMap == nullptr) {
                mesh.material.normalMap = Application::get().getTextures().get("engine_placeholder");
            }

            mesh.hasMaterial = true;
        }
    }

    void setSpecualMap(std::shared_ptr<Texture> texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.specularMap = texture;

            if (mesh.material.diffuseMap == nullptr) {
                mesh.material.diffuseMap = Application::get().getTextures().get("engine_placeholder");
            }

            if (mesh.material.normalMap == nullptr) {
                mesh.material.normalMap = Application::get().getTextures().get("engine_placeholder");
            }

            mesh.hasMaterial = true;
        }
    }

    void setNormalMap(std::shared_ptr<Texture> texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.normalMap = texture;

            if (mesh.material.diffuseMap == nullptr) {
                mesh.material.diffuseMap = Application::get().getTextures().get("engine_placeholder");
            }

            if (mesh.material.specularMap == nullptr) {
                mesh.material.specularMap = Application::get().getTextures().get("engine_placeholder");
            }

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

    bool hasVelocity() { return coordinator().HasComponent<VelocityComponent>(m_Entity); }

    glm::vec3 velocity() {
        if (coordinator().HasComponent<VelocityComponent>(m_Entity)) {
            return coordinator().GetComponent<VelocityComponent>(m_Entity).velocity;
        }

        return glm::vec3(0.0f);
    }

    void velocity(glm::vec3 value) {
        auto &location = coordinator().GetComponent<VelocityComponent>(m_Entity);
        location.velocity = value;
        velocityChange$.dispatch(value);
    }

    glm::vec3 rotationVelocity() {
        if (coordinator().HasComponent<VelocityComponent>(m_Entity)) {
            return coordinator().GetComponent<VelocityComponent>(m_Entity).rotation;
        }

        return glm::vec3(0.0f);
    }

    void rotationVelocity(glm::vec3 value) {
        auto &location = coordinator().GetComponent<VelocityComponent>(m_Entity);
        location.rotation = value;
        rotationVelocityChange$.dispatch(value);
    }

    float directionalSpeed() {
        if (coordinator().HasComponent<VelocityComponent>(m_Entity)) {
            return coordinator().GetComponent<VelocityComponent>(m_Entity).speed;
        }

        return 0.0f;
    }

    void directionalSpeed(float value) {
        auto &location = coordinator().GetComponent<VelocityComponent>(m_Entity);
        location.speed = value;
        directionalSpeedChange$.dispatch(value);
    }

  private:
    Layer &layer() { return Application::get().getLayer("game_area"); }
    Coordinator &coordinator() { return layer().getCoordinator(); }
};

} // namespace Engine
