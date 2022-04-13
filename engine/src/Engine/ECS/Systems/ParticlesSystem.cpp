#include "ParticlesSystem.hpp"

#include "LocationComponent.hpp"
#include "ParticlesComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void ParticlesSystem::OnRemoveComponent(Entity entity) const {
    auto &scene = getScene();
    auto &particles = getCoordinator().GetComponent<ParticlesComponent>(entity);

    scene.removeParticlesEmitter(entity);
    particles.instanced = false;
}

void ParticlesSystem::Attach(ComponentManager &components) const {
    const auto cmpArray = components.GetComponentArray<ParticlesComponent>();
    cmpArray->beforeRemove$.addEventCallback([this](Entity entity) { OnRemoveComponent(entity); });
    cmpArray->active$.addEventCallback([this](Entity entity, bool isActive) {
        if (!isActive) {
            OnRemoveComponent(entity);
        }
    });
}

void ParticlesSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto &particles = components.GetComponent<ParticlesComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        glm::mat4 model(1);
        model = glm::translate(model, location.position);
        model = model * glm::toMat4(glm::quat(location.rotation));

        if (!particles.instanced) {
            scene.addParticlesEmitter(entity, particles.emitter, model);
            particles.instanced = true;
        } else {
            scene.updateParticlesEmitter(entity, particles.emitter, model,
                                         Application::get().getTime().getDeltaSeconds());
        }
    }
}

} // namespace Engine
