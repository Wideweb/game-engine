#pragma once

#include "Entity.hpp"
#include "Layer.hpp"
#include "Window.hpp"

namespace Engine {

class BaseView {
  protected:
    bool m_isVisible = false;

    Layer &gameLayer();
    Layer &toolsLayer();

  public:
    virtual void onAttach();
    virtual void onUpdate();
    virtual void onDraw(int x, int y);
    virtual void onDetach();
    virtual void onMouseEvent(MouseEvent &event);
    virtual bool handleSelection(Entity entity);

    virtual void show();
    virtual void hide();
    bool isVisible();
};

} // namespace Engine
