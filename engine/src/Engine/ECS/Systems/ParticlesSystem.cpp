#include "ParticlesSystem.hpp"

#include "LocationComponent.hpp"
#include "ParticlesComponent.hpp"

namespace Engine {

void ParticlesSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();

    for (auto entity : m_Entities) {
        auto &particles = components.GetComponent<ParticlesComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        glm::mat4 model(1);
        model = glm::translate(model, location.position);
        model = glm::rotate(model, location.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, location.rotation.y, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, location.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        if (particles.instance == NoParticlesEmitterInstance) {
            particles.instance = scene.addParticlesEmitter(particles.emitter, model);
        } else {
            scene.updateParticlesEmitter(particles.instance, model, Application::get().getTime().getDeltaSeconds());
        }
    }
}

} // namespace Engine
