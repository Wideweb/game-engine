#include "ParticlesSystem.hpp"

#include "LocationComponent.hpp"
#include "ParticlesComponent.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine {

void ParticlesSystem::Attach(ComponentManager &components) const {
    components.GetComponentArray<ParticlesComponent>()->beforeRemove$.addEventCallback([this](Entity entity) {
        auto &scene = getScene();
        auto &particles = getCoordinator().GetComponent<ParticlesComponent>(entity);

        scene.removeParticlesEmitter(particles.instance);
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

        if (particles.instance == NoParticlesEmitterInstance) {
            particles.instance = scene.addParticlesEmitter(particles.emitter, model);
        } else {
            scene.updateParticlesEmitter(particles.instance, particles.emitter, model,
                                         Application::get().getTime().getDeltaSeconds());
        }
    }
}

} // namespace Engine
