#pragma once

#include "Collision3D.hpp"
#include "Coordinator.hpp"
#include "Layer.hpp"
#include "MasterRenderer.hpp"
#include "Scene.hpp"
#include "ScriptManager.hpp"
#include "Window.hpp"

#include <string>

namespace Engine {

class Layer {
  protected:
    std::string m_Name;
    Coordinator m_Coordinator;
    Scene m_Scene;
    Collision3D<Entity> m_Collision;
    ScriptManager m_Scripts;

    bool m_Active = false;

  public:
    RenderSettings renderSettings;

    explicit Layer(std::string name);
    virtual ~Layer() = default;

    bool isActive() { return m_Active; }

    void attach();
    void update();
    void draw();
    void detach();

    std::string &getName() { return m_Name; }

    Coordinator &getCoordinator() { return m_Coordinator; }
    Scene &getScene() { return m_Scene; }
    Collision3D<Entity> &getCollision() { return m_Collision; }
    ScriptManager &getScripts() { return m_Scripts; }

    virtual void onAttach() {}
    virtual void onUpdate() {}
    virtual void onDraw() {}
    virtual void onDetach() {}

    virtual void onMouseEvent(MouseEvent &) {}
    virtual void onWindowEvent(WindowEvent &) {}
};

} // namespace Engine