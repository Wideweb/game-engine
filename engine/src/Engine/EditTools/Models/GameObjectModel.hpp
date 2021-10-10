#pragma once

#include "Application.hpp"
#include "BehaviourComponent.hpp"
#include "CollisionComponent.hpp"
#include "DirectedLight.hpp"
#include "DirectedLightComponent.hpp"
#include "Entity.hpp"
#include "EventDispatcher.hpp"
#include "InstancedModel.hpp"
#include "LocationComponent.hpp"
#include "Material.hpp"
#include "Particles.hpp"
#include "ParticlesComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "VelocityComponent.hpp"

#include <glm/vec3.hpp>
#include <string>

namespace Engine {

class GameObjectModel {
  public:
    enum class TransformOrientation { Global = 0, Local = 1 };

  private:
    Entity m_Entity = c_NoEntity;
    TransformOrientation m_TransformOrientation = TransformOrientation::Local;

  public:
    EventDispatcher<glm::vec3> positionChange$;
    EventDispatcher<glm::vec3> rotationChange$;
    EventDispatcher<glm::vec3> localRotationChange$;
    EventDispatcher<glm::vec3, glm::vec3> scaleChange$;
    EventDispatcher<ParticlesConfiguration> particlesChange$;
    EventDispatcher<float> materialSpecularChange$;
    EventDispatcher<float> materialShininessChange$;
    EventDispatcher<glm::vec3> velocityChange$;
    EventDispatcher<glm::vec3> rotationVelocityChange$;
    EventDispatcher<float> directionalSpeedChange$;
    EventDispatcher<DirectedLight> directedLightChange$;

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

        if (hasDirectedLight()) {
            directedLightChange$.dispatch(directedLight());
        }
    }

    Entity entity() { return m_Entity; }

    bool isActive() { return m_Entity != c_NoEntity; }

    void transformOrientation(TransformOrientation value) { m_TransformOrientation = value; }

    TransformOrientation transformOrientation() { return m_TransformOrientation; }

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
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).rotation;
        }

        return glm::vec3(0.0f);
    }

    void rotation(glm::vec3 value) {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

            if (location.rotation == value) {
                return;
            }

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
        return coordinator().HasComponent<Render3DComponent>(m_Entity) &&
               !coordinator().HasComponent<DirectedLightComponent>(m_Entity);
    }

    glm::vec3 scale() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return coordinator().GetComponent<Render3DComponent>(m_Entity).scale;
        }

        return glm::vec3(0.0f);
    }

    void scale(glm::vec3 value) {
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

        glm::vec3 oldValue = render.scale;
        render.scale = value;

        render.updated = true;
        location.updated = true;

        scaleChange$.dispatch(value, oldValue);
    }

    std::string model() { return coordinator().GetComponent<Render3DComponent>(m_Entity).model; }

    bool hasSkelet() { return coordinator().HasComponent<SkeletComponent>(m_Entity); }

    void animation(std::string animation) {
        coordinator().GetComponent<SkeletComponent>(m_Entity).state.play(animation);
    }

    std::string animation() { return coordinator().GetComponent<SkeletComponent>(m_Entity).state.animation(); }

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

    bool hasDirectedLight() { return coordinator().HasComponent<DirectedLightComponent>(m_Entity); }

    DirectedLight directedLight() {
        auto &components = coordinator().GetComponentArray<DirectedLightComponent>()->components();
        if (components.size() == 0) {
            return {};
        }

        return components[0].light;
    }

    void directedLight(DirectedLight value) {
        auto &components = coordinator().GetComponentArray<DirectedLightComponent>()->components();
        if (components.size() == 0 || components[0].light == value) {
            return;
        }

        components[0].light = value;

        directedLightChange$.dispatch(value);
    }

  private:
    Layer &layer() { return Application::get().getLayer("game_area"); }
    Coordinator &coordinator() { return layer().getCoordinator(); }
};

} // namespace Engine
