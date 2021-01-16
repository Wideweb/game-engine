#pragma once

#include "Application.hpp"
#include "System.hpp"

#include <string>

namespace Engine {

class BaseSystem : public System {
  private:
    std::string m_Layer;

  protected:
    Coordinator &getCoordinator() const {
        return Application::get().getLayer(m_Layer).getCoordinator();
    }

    Scene &getScene() const {
        return Application::get().getLayer(m_Layer).getScene();
    }

    Collision3D<Entity> &getCollision() const {
        return Application::get().getLayer(m_Layer).getCollision();
    }

  public:
    explicit BaseSystem(std::string layer) : m_Layer(std::move(layer)) {}
};

} // namespace Engine
