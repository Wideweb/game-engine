#pragma once

#include <functional>
#include <string>

namespace Engine {

enum class EventType {
    None = 0,
    MouseMoved,
    MouseDown,
    WindowResized,
    WindowClosed,
    TextInput
};

struct MouseEvent {
    float x;
    float y;
    EventType type;
    bool handled = false;

    MouseEvent(float x, float y, EventType type) : x(x), y(y), type(type) {}
};

struct WindowEvent {
    EventType type;
    std::string text;

    WindowEvent(EventType type) : type(type) {}
    WindowEvent(EventType type, std::string text) : type(type) {}
};

struct WindowProps {
    int width;
    int height;
};

class Window {
  public:
    template <typename T> using EventCallbackFn = std::function<void(T &)>;

    virtual ~Window() = default;

    virtual void init(const WindowProps &props) = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

    virtual void getDrawableSize(int &width, int &height) const = 0;

    virtual void
    setMouseEventCallback(const EventCallbackFn<MouseEvent> &callback) = 0;
    virtual void
    setWindowEventCallback(const EventCallbackFn<WindowEvent> &callback) = 0;

    virtual void readInput() = 0;
    virtual void swapBuffers() = 0;
    virtual void shutDown() = 0;
    virtual void *getNaviteWindow() const = 0;

    static Window *create();
};

} // namespace Engine
