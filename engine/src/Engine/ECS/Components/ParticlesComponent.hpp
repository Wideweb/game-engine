#pragma once

#include "Particles.hpp"
#include "Scene.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class ParticlesComponent {
  public:
    ParticlesEmitterInstance instance = NoParticlesEmitterInstance;
    ParticlesConfiguration emitter;

    ParticlesComponent() {}
    ParticlesComponent(ParticlesConfiguration emitter) : emitter(emitter) {}
};

} // namespace Engine