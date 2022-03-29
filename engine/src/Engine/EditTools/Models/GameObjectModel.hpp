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
#include "Math.hpp"
#include "ParentComponent.hpp"
#include "Particles.hpp"
#include "ParticlesComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "StaticCollisionComponent.hpp"
#include "TBN.hpp"
#include "TerrainCollisionComponent.hpp"
#include "VelocityComponent.hpp"

#include <glm/gtx/matrix_decompose.hpp>
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
    EventDispatcher<Entity> entityChange$;
    EventDispatcher<glm::vec3> positionChange$;
    EventDispatcher<glm::vec3> rotationChange$;
    EventDispatcher<glm::vec3> renderRotationChange$;
    EventDispatcher<glm::vec3, glm::vec3> scaleChange$;
    EventDispatcher<ParticlesConfiguration> particlesChange$;
    EventDispatcher<float> materialSpecularChange$;
    EventDispatcher<float> materialShininessChange$;
    EventDispatcher<glm::vec3> velocityChange$;
    EventDispatcher<glm::vec3> rotationVelocityChange$;
    EventDispatcher<float> directionalSpeedChange$;
    EventDispatcher<DirectedLight> directedLightChange$;
    EventDispatcher<float> weightChange$;
    EventDispatcher<std::vector<glm::vec3>> collisionChange$;

    void setEntity(Entity entity) {
        m_Entity = entity;

        entityChange$.dispatch(m_Entity);

        if (m_Entity == c_NoEntity) {
            return;
        }

        positionChange$.dispatch(localPosition());
        rotationChange$.dispatch(localRotation());
        renderRotationChange$.dispatch(renderRotation());
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

        if (hasPhysics()) {
            weightChange$.dispatch(weight());
        }

        if (hasCollision()) {
            auto box = collisionBox();
            collisionChange$.dispatch(box);
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

    glm::mat4 parentTransform() {
        glm::mat4 transform(1);

        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return transform;
        }

        std::vector<Entity> ancestors;
        Entity current = m_Entity;
        while (coordinator().HasComponent<ParentComponent>(current)) {
            current = coordinator().GetComponent<ParentComponent>(current).entity;
            ancestors.push_back(current);
        }

        for (int i = ancestors.size() - 1; i >= 0; i--) {
            Entity current = ancestors[i];
            auto &location = coordinator().GetComponent<LocationComponent>(current);
            auto &render = coordinator().GetComponent<Render3DComponent>(current);

            transform = glm::translate(transform, location.position);
            transform = transform * glm::toMat4(glm::quat(location.rotation));
            transform = glm::scale(transform, render.scale);
        }

        return transform;
    }

    glm::mat4 localTransform() {
        glm::mat4 transform(1);

        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return transform;
        }

        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);

        transform = glm::translate(transform, location.position);
        transform = transform * glm::toMat4(glm::quat(location.rotation));
        transform = glm::scale(transform, render.scale);

        return transform;
    }

    glm::mat4 transform() {
        glm::mat4 transform(1);

        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return transform;
        }

        return parentTransform() * localTransform();
    }

    glm::vec3 position() {
        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return glm::vec3(0.0f);
        }

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform(), scale, rotation, position, skew, perspective);

        return position;
    }

    void position(glm::vec3 value) {
        // auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        // location.position = value;
        // location.updated = true;

        // if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
        //     coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        // }

        // if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
        //     coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        // }

        // positionChange$.dispatch(value);
    }

    void localPosition(glm::vec3 value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        location.position = value;
        location.updated = true;

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(value);
    }

    glm::vec3 localPosition() {
        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return glm::vec3(0.0f);
        }

        return coordinator().GetComponent<LocationComponent>(m_Entity).position;
    }

    void move(glm::vec3 value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        glm::mat4 transform = glm::inverse(parentTransform());
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);

        value = rotation * value;

        location.position += value;
        location.updated = true;

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(location.position);
    }

    void moveLocal(glm::vec3 value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        location.position += value;
        location.updated = true;

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(location.position);
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
        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return glm::vec3(0.0f);
        }

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform(), scale, rotation, position, skew, perspective);

        return glm::eulerAngles(rotation);
    }

    void rotation(glm::vec3 value) {
        // if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
        //     auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

        //     if (location.rotation == value) {
        //         return;
        //     }

        //     location.rotation = value;
        //     location.updated = true;

        //     if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
        //         coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        //     }

        //     if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
        //         coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        //     }
        // }

        // rotationChange$.dispatch(value);
    }

    glm::vec3 localRotation() {
        if (!coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return glm::vec3(0.0f);
        }

        return coordinator().GetComponent<LocationComponent>(m_Entity).rotation;
    }

    void localRotation(glm::vec3 value) {
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

            if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
                coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
            }
        }

        rotationChange$.dispatch(value);
    }

    glm::vec3 renderRotation() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return coordinator().GetComponent<Render3DComponent>(m_Entity).rotation;
        }

        return glm::vec3(0.0f);
    }

    void renderRotation(glm::vec3 value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        render.rotation = value;
        render.updated = true;
        renderRotationChange$.dispatch(value);
    }

    bool hasScale() {
        return coordinator().HasComponent<Render3DComponent>(m_Entity) &&
               !coordinator().HasComponent<DirectedLightComponent>(m_Entity);
    }

    glm::vec3 scale() {
        if (!coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return glm::vec3(0.0f);
        }

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform(), scale, rotation, position, skew, perspective);

        return scale;
    }

    void scale(glm::vec3 value) {
        auto parentScale = Math::rescale(glm::vec3(1.0f), localScale(), scale());
        localScale(Math::rescale(glm::vec3(1.0f), parentScale, value));
    }

    glm::vec3 localScale() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return coordinator().GetComponent<Render3DComponent>(m_Entity).scale;
        }

        return glm::vec3(0.0f);
    }

    void localScale(glm::vec3 value) {
        auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);

        glm::vec3 oldValue = scale();
        render.scale = value;
        glm::vec3 newValue = scale();

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            auto &collision = coordinator().GetComponent<StaticCollisionComponent>(m_Entity);
            std::vector<glm::vec3> vertices;
            vertices.reserve(8);
            std::transform(collision.vertices.begin(), collision.vertices.end(), std::back_inserter(vertices),
                           [&](const glm::vec3 &v) { return Math::rescale(v, oldValue, newValue); });
            collisionBox(vertices);
        }

        if (coordinator().HasComponent<CollisionComponent>(m_Entity)) {
            auto &collision = coordinator().GetComponent<CollisionComponent>(m_Entity);
            std::vector<glm::vec3> vertices;
            vertices.reserve(8);
            std::transform(collision.vertices.begin(), collision.vertices.end(), std::back_inserter(vertices),
                           [&](const glm::vec3 &v) { return Math::rescale(v, oldValue, newValue); });
            collisionBox(vertices);
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        render.updated = true;
        location.updated = true;

        scaleChange$.dispatch(newValue, oldValue);
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

    void setDiffuseMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.diffuseMap = texture;

            // if (mesh.material.specularMap.empty()) {
            //     mesh.material.specularMap = Application::get().getTextures().get("engine_placeholder");
            // }

            // if (mesh.material.normalMap.empty()) {
            //     mesh.material.normalMap = Application::get().getTextures().get("engine_placeholder");
            // }

            mesh.hasMaterial = true;
        }
    }

    void setSpecualMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.specularMap = texture;

            // if (mesh.material.diffuseMap.empty()) {
            //     mesh.material.diffuseMap = Application::get().getTextures().get("engine_placeholder");
            // }

            // if (mesh.material.normalMap.empty()) {
            //     mesh.material.normalMap = Application::get().getTextures().get("engine_placeholder");
            // }

            mesh.hasMaterial = true;
        }
    }

    void setNormalMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.normalMap = texture;

            // if (mesh.material.diffuseMap.empty()) {
            //     mesh.material.diffuseMap = Application::get().getTextures().get("engine_placeholder");
            // }

            // if (mesh.material.specularMap.empty()) {
            //     mesh.material.specularMap = Application::get().getTextures().get("engine_placeholder");
            // }

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

    bool hasTerrainCollision() { return coordinator().HasComponent<TerrainCollisionComponent>(m_Entity); }

    bool hasCollision() {
        return coordinator().HasComponent<StaticCollisionComponent>(m_Entity) ||
               coordinator().HasComponent<CollisionComponent>(m_Entity);
    }

    std::vector<glm::vec3> collisionBox() {
        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            return coordinator().GetComponent<StaticCollisionComponent>(m_Entity).vertices;
        }

        if (coordinator().HasComponent<CollisionComponent>(m_Entity)) {
            return coordinator().GetComponent<CollisionComponent>(m_Entity).vertices;
        }

        return {};
    }

    void collisionBox(std::vector<glm::vec3> value) {
        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            auto &collision = coordinator().GetComponent<StaticCollisionComponent>(m_Entity);

            collision.vertices = value;
            collision.updated = true;

            collisionChange$.dispatch(value);
        }

        if (coordinator().HasComponent<CollisionComponent>(m_Entity)) {
            auto &collision = coordinator().GetComponent<CollisionComponent>(m_Entity).vertices = value;

            collisionChange$.dispatch(value);
        }
    }

    bool hasPhysics() { return coordinator().HasComponent<PhysicsComponent>(m_Entity); }

    float weight() {
        if (coordinator().HasComponent<PhysicsComponent>(m_Entity)) {
            return coordinator().GetComponent<PhysicsComponent>(m_Entity).weight;
        }

        return 0;
    }

    void weight(float value) {
        if (coordinator().HasComponent<PhysicsComponent>(m_Entity)) {
            coordinator().GetComponent<PhysicsComponent>(m_Entity).weight = value;
            weightChange$.dispatch(value);
        }
    }

    void updateTerrain(std::vector<Mesh::Vertex> vertices) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        Render3D::Utils::tbn(vertices, model->meshes[0].indices);
        model->meshes[0].vertices = vertices;
        model->update();

        coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
    }

    void updateTerrain(std::vector<Mesh::Vertex> vertices, std::vector<unsigned int> indices, int columns, int rows) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);

        Render3D::Utils::tbn(vertices, indices);

        model->meshes[0].vertices = vertices;
        model->meshes[0].indices = indices;
        model->update();

        auto &terrain = coordinator().GetComponent<TerrainCollisionComponent>(m_Entity);
        terrain.columns = columns;
        terrain.rows = rows;
        terrain.updated = true;
    }

    void parent(Entity entity) {
        if (coordinator().HasComponent<ParentComponent>(m_Entity)) {
            coordinator().GetComponent<ParentComponent>(m_Entity).setEntity(entity);
        } else {
            coordinator().AddComponent<ParentComponent>(m_Entity, ParentComponent(entity));
        }

        positionChange$.dispatch(localPosition());
        rotationChange$.dispatch(localRotation());
        scaleChange$.dispatch(scale(), scale());
    }

  private:
    Layer &layer() { return Application::get().getLayer("game_area"); }
    Coordinator &coordinator() { return layer().getCoordinator(); }
};

} // namespace Engine
