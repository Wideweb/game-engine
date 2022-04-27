#include "SDLWindow.hpp"

#include "glad/glad.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string_view>

namespace Engine {

static std::ostream &operator<<(std::ostream &out, const SDL_version &v) {
    out << static_cast<int>(v.major) << '.';
    out << static_cast<int>(v.minor) << '.';
    out << static_cast<int>(v.patch);
    return out;
}

SDLWindow::SDLWindow(const WindowProps &props) {
    using namespace std;
    using namespace std::string_view_literals;

    m_Props = props;

    SDL_version compiled = {0, 0, 0};
    SDL_version linked = {0, 0, 0};

    SDL_VERSION(&compiled)
    SDL_GetVersion(&linked);

    if (SDL_COMPILEDVERSION != SDL_VERSIONNUM(linked.major, linked.minor, linked.patch)) {
        cerr << "warning: SDL2 compiled and linked version mismatch: " << compiled << " " << linked << endl;
    }

    const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (init_result != 0) {
        const char *err_message = SDL_GetError();
        cerr << "error: failed call SDL_Init: " << err_message << endl;
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    m_Window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.width, props.height,
                                ::SDL_WINDOW_OPENGL | ::SDL_WINDOW_RESIZABLE);

    if (m_Window == nullptr) {
        const char *err_message = SDL_GetError();
        cerr << "error: failed call SDL_CreateWindow: " << err_message << endl;
        SDL_Quit();
        return;
    }

    ///////////////////////
    // OpenGl /////////////
    ///////////////////////

    int gl_major_ver = 3;
    int gl_minor_ver = 2;
    int gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

    std::string_view platform = SDL_GetPlatform();

    auto list = {"Windows"sv, "Mac OS X"sv, "Linux"sv};
    auto it = find(begin(list), end(list), platform);
    if (it != end(list)) {
        gl_minor_ver = 3;
        gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    if (props.antialiasing) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    }

    m_Context = SDL_GL_CreateContext(reinterpret_cast<SDL_Window *>(m_Window));

    if (m_Context == nullptr) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        m_Context = SDL_GL_CreateContext(reinterpret_cast<SDL_Window *>(m_Window));
    }
    assert(m_Context != nullptr);

    int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
    assert(result == 0);

    result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
    assert(result == 0);

    if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0) {
        cerr << "failed to initialize glad" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

SDLWindow::~SDLWindow() {}

int SDLWindow::getWidth() const { return m_Props.width; }

int SDLWindow::getHeight() const { return m_Props.height; }

glm::vec2 SDLWindow::getSize() const { return glm::vec2(m_Props.width, m_Props.height); };

void SDLWindow::setMouseEventCallback(const EventCallbackFn<MouseEvent &> &callback) {
    m_mouseEventCallback = callback;
}

void SDLWindow::setWindowEventCallback(const EventCallbackFn<WindowEvent &> &callback) {
    m_windowEventCallback = callback;
}

void SDLWindow::setNativeEventCallback(const EventCallbackFn<void *> &callback) { m_nativeEventCallback = callback; }

void SDLWindow::readInput() {
    {
        WindowEvent event(EventType::TextInput, "");
        m_windowEventCallback(event);
    }
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        m_MouseEvent.type = EventType::None;

        if (e.type == SDL_QUIT) {
            WindowEvent event(EventType::WindowClosed);
            m_windowEventCallback(event);
        } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {

            SDL_GetWindowSize(m_Window, &m_Props.width, &m_Props.height);

            WindowEvent event(EventType::WindowResized);
            m_windowEventCallback(event);
            break;
        } else if (e.type == SDL_MOUSEMOTION) {
            m_MouseEvent = MouseEvent(e.motion.x, m_Props.height - e.motion.y, EventType::MouseMoved);
            m_mouseEventCallback(m_MouseEvent);
        } else if (e.type == SDL_MOUSEWHEEL) {
            m_MouseEvent = MouseEvent(e.wheel.x, e.wheel.y, EventType::MouseWheel);
            m_mouseEventCallback(m_MouseEvent);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            m_MouseEvent = MouseEvent(e.motion.x, m_Props.height - e.motion.y, EventType::MouseDown);
            m_mouseEventCallback(m_MouseEvent);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            m_MouseEvent = MouseEvent(e.motion.x, m_Props.height - e.motion.y, EventType::MouseUp);
            m_mouseEventCallback(m_MouseEvent);
        } else if (e.type == SDL_TEXTINPUT) {
            WindowEvent event(EventType::TextInput, e.text.text);
            m_windowEventCallback(event);
        }
        // else if (e.type == SDL_MULTIGESTURE) {
        //     const SDL_MultiGestureEvent *mge = &e.mgesture;
        //     int nFingers = SDL_GetNumTouchFingers(mge->touchId);
        //     if (nFingers == 2) {
        //         std::cout << "SDL_MULTIGESTURE" << std::endl;
        //     }
        // }

        if (m_nativeEventCallback) {
            m_nativeEventCallback(&e);
        }
    }
}

void SDLWindow::swapBuffers() { SDL_GL_SwapWindow(m_Window); }

void *SDLWindow::getNaviteWindow() const { return m_Window; }

void *SDLWindow::getContext() const { return m_Context; }

MouseEvent &SDLWindow::getMouseEvent() { return m_MouseEvent; }

void SDLWindow::getDrawableSize(int &width, int &height) const { SDL_GL_GetDrawableSize(m_Window, &width, &height); }

void SDLWindow::shutDown() {
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

} // namespace Engine
