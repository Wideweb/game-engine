#pragma once

#include "Window.hpp"

#include <SDL2/SDL.h>

namespace Engine {

class SDLWindow : public Window {
  private:
    SDL_Window *m_Window;
    SDL_GLContext m_Context;
    WindowProps m_Props;
    EventCallbackFn<MouseEvent &> m_mouseEventCallback;
    EventCallbackFn<WindowEvent &> m_windowEventCallback;
    EventCallbackFn<void *> m_nativeEventCallback;

  public:
    SDLWindow(const WindowProps &props);
    virtual ~SDLWindow();

    virtual int getWidth() const override;
    virtual int getHeight() const override;
    virtual glm::vec2 getSize() const override;

    virtual void getDrawableSize(int &width, int &height) const override;

    virtual void setMouseEventCallback(const EventCallbackFn<MouseEvent &> &callback) override;

    virtual void setWindowEventCallback(const EventCallbackFn<WindowEvent &> &callback) override;

    virtual void setNativeEventCallback(const EventCallbackFn<void *> &callback) override;

    virtual void readInput() override;
    virtual void swapBuffers() override;
    virtual void shutDown() override;
    virtual void *getNaviteWindow() const override;
    virtual void *getContext() const override;
};

} // namespace Engine
