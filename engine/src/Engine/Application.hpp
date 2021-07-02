#pragma once

#include "Camera.hpp"
#include "EventHandler.hpp"
#include "Input.hpp"
#include "Layer.hpp"
#include "MasterRenderer.hpp"
#include "ModelManager.hpp"
#include "MousePicker.hpp"
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

struct ApplicationSettings {
    bool edit = false;
    bool hdr = false;
    bool antialiasing = false;
    bool width = false;
};

class Application {
  private:
    static Application *s_Instance;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Input> m_Input;
    std::unique_ptr<MasterRenderer> m_Render;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<TextureManager> m_Texture;
    std::unique_ptr<EventHandler> m_EventHandler;
    std::unique_ptr<SoundMixer> m_Sound;
    std::unique_ptr<MousePicker> m_MousePicker;
    std::list<std::shared_ptr<Layer>> m_LayerStack;
    std::unordered_map<std::string, std::list<std::shared_ptr<Layer>>::iterator> m_NameToLayer;
    Time m_Time;
    ModelManager m_Models;

    bool m_Running = true;

  public:
    Application(ApplicationSettings settings);
    virtual ~Application();

    void onMouseEvent(MouseEvent &e);
    void onWindowEvent(WindowEvent &e);

    void initialize();
    void run();
    void stop();

    template <typename T> T &addLayer(const std::string &label) {
        auto layer = std::make_shared<T>(label);
        m_LayerStack.push_back(layer);
        m_NameToLayer[label] = std::prev(m_LayerStack.end());
        layer->attach();
        return *layer;
    }

    void removeLayer(const std::string &label) {
        auto layerIt = m_NameToLayer[label];
        (*layerIt)->detach();
        m_LayerStack.erase(layerIt);
        m_NameToLayer.erase(label);
    }

    template <typename T> void reloadLayer(const std::string &label) {
        auto renderSettings = (*m_NameToLayer[label])->renderSettings;
        removeLayer(label);
        addLayer<T>(label);
    }

    Window &getWindow() { return *m_Window; }
    SoundMixer &getSound() { return *m_Sound; }
    Input &getInput() { return *m_Input; }
    MousePicker &getMousePicker() { return *m_MousePicker; }
    MasterRenderer &getRender() { return *m_Render; }
    Camera &getCamera() { return *m_Camera; }
    Time &getTime() { return m_Time; }
    TextureManager &getTextures() { return *m_Texture; }
    ModelManager &getModels() { return m_Models; }
    EventHandler &getEventHandler() { return *m_EventHandler; }
    Layer &getLayer(const std::string &label) { return **m_NameToLayer[label]; }

    static Application &get() { return *s_Instance; }
};

Application *createApplication(ApplicationSettings);

} // namespace Engine
