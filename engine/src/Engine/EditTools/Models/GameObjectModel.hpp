#pragma once

#include "Application.hpp"
#include "BehaviourComponent.hpp"
#include "CollisionComponent.hpp"
#include "DestroyComponent.hpp"
#include "DirectedLight.hpp"
#include "DirectedLightComponent.hpp"
#include "Entity.hpp"
#include "EventDispatcher.hpp"
#include "InstancedModel.hpp"
#include "Location2DComponent.hpp"
#include "LocationComponent.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "ParentComponent.hpp"
#include "Particles.hpp"
#include "ParticlesComponent.hpp"
#include "PhysicsComponent.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "SkinnedModel.hpp"
#include "StaticCollisionComponent.hpp"
#include "TBN.hpp"
#include "TerrainCollisionComponent.hpp"
#include "Text2DComponent.hpp"
#include "TextureLoader.hpp"
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
    TransformOrientation m_TransformOrientation = TransformOrientation::Global;

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

        if (hasRender() && hasModel()) {
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

    std::string entityName() { return coordinator().GetEntityName(m_Entity); }

    bool entityName(std::string name) { return coordinator().ChangeEntityName(m_Entity, name); }

    bool isActive() { return m_Entity != c_NoEntity; }

    void transformOrientation(TransformOrientation value) { m_TransformOrientation = value; }

    TransformOrientation transformOrientation() { return m_TransformOrientation; }

    bool hasTransform2D() { return coordinator().HasComponent<Location2DComponent>(m_Entity); }

    bool hasTransform() { return coordinator().HasComponent<LocationComponent>(m_Entity); }

    glm::mat4 parentTransform() {
        glm::mat4 transform(1);

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return LocationComponent::getParentTransform(m_Entity, coordinator().GetComponentManager());
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            return Location2DComponent::getParentTransform(m_Entity, coordinator().GetComponentManager());
        }

        return transform;
    }

    glm::mat4 localTransform() {
        glm::mat4 transform(1);

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return LocationComponent::getLocalTransform(m_Entity, coordinator().GetComponentManager());
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            return Location2DComponent::getLocalTransform(m_Entity, coordinator().GetComponentManager());
        }

        return transform;
    }

    glm::mat4 transform() {
        glm::mat4 transform(1);

        if (!coordinator().HasComponent<LocationComponent>(m_Entity) &&
            !coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            return transform;
        }

        return parentTransform() * localTransform();
    }

    glm::vec3 position() {
        if (!coordinator().HasComponent<LocationComponent>(m_Entity) &&
            !coordinator().HasComponent<Location2DComponent>(m_Entity)) {
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
        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<Location2DComponent>(m_Entity);
            location.position = value;
            location.updated = true;
        }

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);
            location.position = value;
            location.updated = true;
        }

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(value);
    }

    glm::vec3 localPosition() {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).position;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            return glm::vec3(coordinator().GetComponent<Location2DComponent>(m_Entity).position, 0.0f);
        }

        return glm::vec3(0.0f);
    }

    void move(glm::vec3 value) {
        glm::mat4 transform = glm::inverse(parentTransform());
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);

        value = rotation * value;
        glm::vec3 newPosition;

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);
            location.position += value;
            location.updated = true;
            newPosition = location.position;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<Location2DComponent>(m_Entity);
            location.position += glm::vec2(value);
            location.updated = true;
            newPosition = glm::vec3(location.position, 0.0f);
        }

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(newPosition);
    }

    void moveLocal(glm::vec3 value) {
        glm::vec3 newPosition;

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);
            location.position += value;
            location.updated = true;
            newPosition = location.position;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<Location2DComponent>(m_Entity);
            location.position += glm::vec2(value);
            location.updated = true;
            newPosition = glm::vec3(location.position, 0.0f);
        }

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
        }

        positionChange$.dispatch(newPosition);
    }

    glm::vec3 rotation() {
        if (!coordinator().HasComponent<LocationComponent>(m_Entity) &&
            !coordinator().HasComponent<Location2DComponent>(m_Entity)) {
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

    glm::vec3 localRotation() {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).rotation;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            return glm::vec3(0.0f, 0.0f, coordinator().GetComponent<Location2DComponent>(m_Entity).rotation);
        }

        return glm::vec3(0.0f);
    }

    void localRotation(glm::vec3 value) {
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

            if (location.rotation == value) {
                return;
            }

            location.rotation = value;
            location.updated = true;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<Location2DComponent>(m_Entity);

            if (location.rotation == value.z) {
                return;
            }

            location.rotation = value.z;
            location.updated = true;
        }

        if (coordinator().HasComponent<StaticCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<StaticCollisionComponent>(m_Entity).updated = true;
        }

        if (coordinator().HasComponent<TerrainCollisionComponent>(m_Entity)) {
            coordinator().GetComponent<TerrainCollisionComponent>(m_Entity).updated = true;
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
        return coordinator().HasComponent<LocationComponent>(m_Entity) &&
               !coordinator().HasComponent<DirectedLightComponent>(m_Entity);
    }

    glm::vec3 scale() {
        if (!coordinator().HasComponent<LocationComponent>(m_Entity) &&
            !coordinator().HasComponent<Location2DComponent>(m_Entity)) {
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
        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            return coordinator().GetComponent<LocationComponent>(m_Entity).scale;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            return glm::vec3(coordinator().GetComponent<Location2DComponent>(m_Entity).scale, 1.0f);
        }

        return glm::vec3(0.0f);
    }

    void localScale(glm::vec3 value) {
        if (value.x <= 0.01) {
            value.x = 0.01;
        }

        if (value.y <= 0.01) {
            value.y = 0.01;
        }

        if (value.z <= 0.01) {
            value.z = 0.01;
        }

        glm::vec3 oldValue, newValue;

        if (coordinator().HasComponent<LocationComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<LocationComponent>(m_Entity);

            oldValue = scale();
            location.scale = value;
            newValue = scale();
            location.updated = true;
        }

        if (coordinator().HasComponent<Location2DComponent>(m_Entity)) {
            auto &location = coordinator().GetComponent<Location2DComponent>(m_Entity);

            oldValue = scale();
            location.scale = glm::vec2(value);
            newValue = scale();
            location.updated = true;
        }

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

        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            coordinator().GetComponent<Render3DComponent>(m_Entity).updated = true;
        }

        scaleChange$.dispatch(newValue, oldValue);
    }

    glm::vec3 renderScale() {
        if (coordinator().HasComponent<Render3DComponent>(m_Entity)) {
            return coordinator().GetComponent<Render3DComponent>(m_Entity).scale;
        }

        return glm::vec3(0.0f);
    }

    void renderScale(glm::vec3 value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        render.scale = value;
        render.updated = true;
    }

    std::string model() { return coordinator().GetComponent<Render3DComponent>(m_Entity).model; }

    bool hasSkelet() { return coordinator().HasComponent<SkeletComponent>(m_Entity); }

    void animation(std::string animation) {
        coordinator().GetComponent<SkeletComponent>(m_Entity).state.play(animation, true);
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

    template <typename TModel> void setDiffuseMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = models.GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            if (mesh.material.diffuseMap.id != TextureLoader::placeholder().id) {
                mesh.material.diffuseMap.free();
            }
            mesh.material.diffuseMap = texture;
        }
    }

    void setDiffuseMap(Texture texture) {
        setDiffuseMap<InstancedModel>(texture);
        setDiffuseMap<SkinnedModel>(texture);
    }

    template <typename TModel> void setSpecularMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            if (mesh.material.specularMap.id != TextureLoader::placeholder().id) {
                mesh.material.specularMap.free();
            }
            mesh.material.specularMap = texture;
        }
    }

    void setSpecularMap(Texture texture) {
        setSpecularMap<InstancedModel>(texture);
        setSpecularMap<SkinnedModel>(texture);
    }

    template <typename TModel> void setNormalMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            if (mesh.material.normalMap.id != TextureLoader::placeholder().id) {
                mesh.material.normalMap.free();
            }
            mesh.material.normalMap = texture;
        }
    }

    void setNormalMap(Texture texture) {
        setNormalMap<InstancedModel>(texture);
        setNormalMap<SkinnedModel>(texture);
    }

    template <typename TModel> void setMetallicMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            if (mesh.material.metallicMap.id != TextureLoader::placeholder().id) {
                mesh.material.metallicMap.free();
            }
            mesh.material.metallicMap = texture;
        }
    }

    void setMetallicMap(Texture texture) {
        setMetallicMap<InstancedModel>(texture);
        setMetallicMap<SkinnedModel>(texture);
    }

    template <typename TModel> void setRoughnessMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            if (mesh.material.roughnessMap.id != TextureLoader::placeholder().id) {
                mesh.material.roughnessMap.free();
            }
            mesh.material.roughnessMap = texture;
        }
    }

    void setRoughnessMap(Texture texture) {
        setRoughnessMap<InstancedModel>(texture);
        setRoughnessMap<SkinnedModel>(texture);
    }

    template <typename TModel> void setAmbientOcclusionMap(Texture texture) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            if (mesh.material.ambientOcclusionMap.id != TextureLoader::placeholder().id) {
                mesh.material.ambientOcclusionMap.free();
            }
            mesh.material.ambientOcclusionMap = texture;
        }
    }

    void setAmbientOcclusionMap(Texture texture) {
        setAmbientOcclusionMap<InstancedModel>(texture);
        setAmbientOcclusionMap<SkinnedModel>(texture);
    }

    template <typename TModel> void materialSpecular(float value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.specular = value;
        }

        materialSpecularChange$.dispatch(value);
    }

    void materialSpecular(float value) {
        materialSpecular<InstancedModel>(value);
        materialSpecular<SkinnedModel>(value);
    }

    template <typename TModel> void materialShininess(float value) {
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);
        const auto &models = Application::get().getModels();

        if (!models.Is<TModel>(render.model)) {
            return;
        }

        const auto &model = Application::get().getModels().GetModel<TModel>(render.model);
        for (auto &mesh : model->meshes) {
            mesh.material.shininess = value;
        }

        materialShininessChange$.dispatch(value);
    }

    void materialShininess(float value) {
        materialShininess<InstancedModel>(value);
        materialShininess<SkinnedModel>(value);
    }

    bool hasRender() { return coordinator().HasComponent<Render3DComponent>(m_Entity); }

    bool hasText2D() { return coordinator().HasComponent<Text2DComponent>(m_Entity); }

    bool hasModel() { return hasRender() && !coordinator().GetComponent<Render3DComponent>(m_Entity).model.empty(); }

    bool isRenderActive() { return coordinator().IsComponentActive<Render3DComponent>(m_Entity); }

    auto &render() { return coordinator().GetComponent<Render3DComponent>(m_Entity); }

    void set3DModel(const std::string &model) {
        const auto &models = Application::get().getModels();

        render().setModel(model);

        if (hasSkelet() && (!hasModel() || !models.Is<SkinnedModel>(model))) {
            animation("");
        }
        if (!hasSkelet() && hasModel() && models.Is<SkinnedModel>(model)) {
            coordinator().AddComponent<SkeletComponent>(m_Entity, SkeletComponent());
        }
    }

    const Material &material() {
        const auto &models = Application::get().getModels();
        auto &render = coordinator().GetComponent<Render3DComponent>(m_Entity);

        if (models.Is<SkinnedModel>(render.model)) {
            return models.GetModel<SkinnedModel>(render.model)->meshes[0].material;
        }
        return models.GetModel<InstancedModel>(render.model)->meshes[0].material;
    }

    bool hasBehaviour() { return coordinator().HasComponent<BehaviourComponent>(m_Entity); }

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

    BehaviourComponent &behaviour() { return coordinator().GetComponent<BehaviourComponent>(m_Entity); }

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

    bool isTerrainCollisionActive() { return coordinator().IsComponentActive<TerrainCollisionComponent>(m_Entity); }

    bool hasCollision() {
        return coordinator().HasComponent<StaticCollisionComponent>(m_Entity) ||
               coordinator().HasComponent<CollisionComponent>(m_Entity);
    }

    bool isCollisionActive() {
        return coordinator().IsComponentActive<StaticCollisionComponent>(m_Entity) ||
               coordinator().IsComponentActive<CollisionComponent>(m_Entity);
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

    void destroy() {
        if (!coordinator().HasComponent<DestroyComponent>(m_Entity)) {
            coordinator().AddComponent<DestroyComponent>(m_Entity, DestroyComponent());
        }
    }

    void update() {
        if (!coordinator().HasEntity(m_Entity)) {
            m_Entity = c_NoEntity;
        }
    }

    Layer &layer() { return Application::get().getLayer("game_area"); }

    Coordinator &coordinator() { return layer().getCoordinator(); }
};

} // namespace Engine
