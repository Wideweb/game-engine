#include "Application.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

namespace Engine {

Application *Application::s_Instance = nullptr;

Application::Application() {
    m_Window = std::unique_ptr<Window>(Window::create({960, 540}));
    m_Window->setMouseEventCallback(
        std::bind(&Application::onMouseEvent, this, std::placeholders::_1));
    m_Window->setWindowEventCallback(
        std::bind(&Application::onWindowEvent, this, std::placeholders::_1));

    m_Input = std::unique_ptr<Input>(Input::create());

    m_Render = std::make_unique<Render>(960 * 2, 540 * 2);

    m_Camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 0.0f, 1.0f),
                                                  glm::vec3(0.0f, 1.0f, 0.0f),
                                                  glm::vec3(0.0, 0.0f, -1.0f)));
    m_Camera->setSize(960, 540);
    m_Camera->setPerspective(glm::radians(45.0f), 0.01f, 100.0f);
    m_Camera->setProjection(Camera::Projection::PERSPECTIVE);

    m_Texture = std::unique_ptr<TextureManager>(new TextureManager());

    m_EventHandler = std::unique_ptr<EventHandler>(new EventHandler());

    m_Sound = std::unique_ptr<SoundMixer>(SoundMixer::create());

    s_Instance = this;
}

glm::vec2 Application::getScreenFix() {
    return glm::vec2(960.0f / m_Window->getWidth(),
                     540.0f / m_Window->getHeight());
}

void Application::run() {
    m_Render->setClearColor(0.0f, 0.1f, 0.1f, 1.0f);
    m_Time.tick();

    while (m_Running) {
        m_Time.tick();

        m_Window->readInput();
        m_Input->update();

        m_Render->clear();
        for (auto layer : m_LayerStack) {
            layer->getScene().clear();
            layer->update();
            m_Render->draw(layer->getScene(), m_Models, *m_Camera);
        }

        m_Window->swapBuffers();
    }
}

void Application::stop() { m_Running = false; }

void Application::onMouseEvent(MouseEvent &e) {
    for (size_t i = m_LayerStack.size() - 1; i > 0; i--) {
        auto layer = m_LayerStack[i];
        layer->onMouseEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void Application::onWindowEvent(WindowEvent &e) {
    if (e.type == EventType::WindowResized) {
        // m_Render->setViewport(m_Window->getWidth(), m_Window->getHeight());
    }

    for (auto layer : m_LayerStack) {
        layer->onWindowEvent(e);
    }

    if (e.type == EventType::WindowClosed) {
        stop();
    }
}

Application::~Application() {
    for (auto layer : m_LayerStack) {
        layer->onDetach();
    }
    m_Window->shutDown();
}

} // namespace Engine
