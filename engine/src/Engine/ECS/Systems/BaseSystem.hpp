#pragma once

#include "Application.hpp"
#include "System.hpp"

#include <string>

namespace Engine {

class BaseSystem : public System {
  private:
    std::string m_Layer;

  protected:
    Layer &getLayer() const { return Application::get().getLayer(m_Layer); }

    Coordinator &getCoordinator() const { return getLayer().getCoordinator(); }

    Scene &getScene() const { return getLayer().getScene(); }

    Collision3D<Entity> &getCollision() const {
        return getLayer().getCollision();
    }

    ScriptManager &getScripts() const { return getLayer().getScripts(); }

  public:
    explicit BaseSystem(std::string layer) : m_Layer(std::move(layer)) {}
};

} // namespace Engine
