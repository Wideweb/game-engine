#pragma once

#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Input.hpp"
#include "Layer.hpp"
#include "ModelManager.hpp"
#include "Render.hpp"
#include "SoundMixer.hpp"
#include "TextureManager.hpp"
#include "Time.hpp"
#include "Window.hpp"

#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine {

class Application {
  private:
    static Application *s_Instance;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Input> m_Input;
    std::unique_ptr<Render> m_Render;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<TextureManager> m_Texture;
    std::unique_ptr<EventHandler> m_EventHandler;
    std::unique_ptr<SoundMixer> m_Sound;
    std::list<std::shared_ptr<Layer>> m_LayerStack;
    std::unordered_map<std::string, std::list<std::shared_ptr<Layer>>::iterator>
        m_NameToLayer;
    Time m_Time;
    ModelManager m_Models;

    bool m_Running = true;

  public:
    Application();
    virtual ~Application();

    void onMouseEvent(MouseEvent &e);
    void onWindowEvent(WindowEvent &e);

    void initialize();
    void run();
    void stop();

    template <typename T> void addLayer(const std::string &label) {
        auto layer = std::make_shared<T>(label);
        m_LayerStack.push_back(layer);
        m_NameToLayer[label] = std::prev(m_LayerStack.end());
        layer->attach();
    }

    void removeLayer(const std::string &label) {
        auto layerIt = m_NameToLayer[label];
        (*layerIt)->detach();
        m_LayerStack.erase(layerIt);
        m_NameToLayer.erase(label);
    }

    template <typename T> void reloadLayer(const std::string &label) {
        removeLayer(label);
        addLayer<T>(label);
    }

    Window &getWindow() { return *m_Window; }
    SoundMixer &getSound() { return *m_Sound; }
    Input &getInput() { return *m_Input; }
    Render &getRender() { return *m_Render; }
    Camera &getCamera() { return *m_Camera; }
    Time &getTime() { return m_Time; }
    TextureManager &getTextures() { return *m_Texture; }
    ModelManager &getModels() { return m_Models; }
    EventHandler &getEventHandler() { return *m_EventHandler; }
    Layer &getLayer(const std::string &label) { return **m_NameToLayer[label]; }
    glm::vec2 getScreenFix();

    static Application &get() { return *s_Instance; }
};

Application *createApplication();

} // namespace Engine
