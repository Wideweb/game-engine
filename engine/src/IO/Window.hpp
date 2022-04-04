#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <string>

namespace Engine {

enum class EventType { None = 0, MouseMoved, MouseDown, MouseUp, MouseWheel, WindowResized, WindowClosed, TextInput };

struct MouseEvent {
    float x;
    float y;
    EventType type;
    bool handled = false;
    void *data;

    MouseEvent(float x, float y, EventType type) : x(x), y(y), type(type) {}
    MouseEvent() : MouseEvent(0.0f, 0.0f, EventType::None) {}
};

struct WindowEvent {
    EventType type;
    std::string text;

    WindowEvent(EventType type) : type(type) {}
    WindowEvent(EventType type, std::string text) : type(type), text(std::move(text)) {}
};

struct WindowProps {
    int width;
    int height;
    bool antialiasing;
};

class Window {
  public:
    template <typename T> using EventCallbackFn = std::function<void(T)>;

    virtual ~Window() = default;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual glm::vec2 getSize() const = 0;

    virtual void getDrawableSize(int &width, int &height) const = 0;

    virtual void setMouseEventCallback(const EventCallbackFn<MouseEvent &> &callback) = 0;
    virtual void setWindowEventCallback(const EventCallbackFn<WindowEvent &> &callback) = 0;
    virtual void setNativeEventCallback(const EventCallbackFn<void *> &callback) = 0;

    virtual void readInput() = 0;
    virtual void swapBuffers() = 0;
    virtual void shutDown() = 0;
    virtual void *getNaviteWindow() const = 0;
    virtual void *getContext() const = 0;
    virtual MouseEvent &getMouseEvent() = 0;

    static Window *create(const WindowProps &props);
};

} // namespace Engine
