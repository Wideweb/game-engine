#pragma once

#include "Coordinator.hpp"
#include "Layer.hpp"
#include "Window.hpp"

#include <string>

namespace Engine {

class Layer {
  protected:
    Coordinator m_Coordinator;

  public:
    Layer();
    virtual ~Layer() = default;

    void update();

    Coordinator &getCoordinator() { return m_Coordinator; }

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate() {}

    virtual void onMouseEvent(MouseEvent &) {}
    virtual void onWindowEvent(WindowEvent &) {}
};

} // namespace Engine