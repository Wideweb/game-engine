#pragma once

#include "Collision3D.hpp"
#include "Coordinator.hpp"
#include "Layer.hpp"
#include "Scene.hpp"
#include "Window.hpp"

#include <string>

namespace Engine {

class Layer {
  protected:
    std::string m_Name;
    Coordinator m_Coordinator;
    Scene m_Scene;
    Collision3D<Entity> m_Collision;

  public:
    explicit Layer(std::string name);
    virtual ~Layer() = default;

    void update();

    std::string &getName() { return m_Name; }

    Coordinator &getCoordinator() { return m_Coordinator; }
    Scene &getScene() { return m_Scene; }
    Collision3D<Entity> &getCollision() { return m_Collision; }

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate() {}

    virtual void onMouseEvent(MouseEvent &) {}
    virtual void onWindowEvent(WindowEvent &) {}
};

} // namespace Engine