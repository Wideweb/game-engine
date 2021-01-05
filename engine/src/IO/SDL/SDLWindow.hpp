#pragma once

#include "Window.hpp"

#include <SDL.h>

namespace Engine {

class SDLWindow : public Window {
  private:
    SDL_Window *m_Window;
    SDL_GLContext m_Context;
    WindowProps m_Props;
    EventCallbackFn<MouseEvent> m_mouseEventCallback;
    EventCallbackFn<WindowEvent> m_windowEventCallback;

  public:
    SDLWindow(const WindowProps &props);
    virtual ~SDLWindow();

    virtual int getWidth() const override;
    virtual int getHeight() const override;

    virtual void getDrawableSize(int &width, int &height) const override;

    virtual void
    setMouseEventCallback(const EventCallbackFn<MouseEvent> &callback) override;

    virtual void setWindowEventCallback(
        const EventCallbackFn<WindowEvent> &callback) override;

    virtual void readInput() override;
    virtual void swapBuffers() override;
    virtual void shutDown() override;
    virtual void *getNaviteWindow() const override;
};

} // namespace Engine
