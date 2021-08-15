#pragma once

#include "Application.hpp"
#include "DirectedLight.hpp"
#include "DirectedLightComponent.hpp"
#include "EventDispatcher.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class DirectedLightModel {
  private:
    bool m_IsActive = false;

  public:
    EventDispatcher<DirectedLight> change$;

    bool isActive() {
        auto &components = coordinator().GetComponentArray<DirectedLightComponent>()->components();
        bool isActive = components.size() > 0;

        if (isActive && m_IsActive != isActive) {
            change$.dispatch(components[0].light);
        }
        m_IsActive = isActive;

        return m_IsActive;
    }

    DirectedLight light() {
        auto &components = coordinator().GetComponentArray<DirectedLightComponent>()->components();
        if (components.size() == 0) {
            return {};
        }

        return components[0].light;
    }

    void light(DirectedLight value) {
        auto &components = coordinator().GetComponentArray<DirectedLightComponent>()->components();
        if (components.size() == 0) {
            return;
        }

        components[0].light = value;

        change$.dispatch(value);
    }

  private:
    Coordinator &coordinator() { return Application::get().getLayer("game_area").getCoordinator(); }
};

} // namespace Engine
