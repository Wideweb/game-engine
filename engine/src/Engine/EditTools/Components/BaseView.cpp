#include "BaseView.hpp"

#include "Application.hpp"

namespace Engine {

Layer &BaseView::gameLayer() { return Application::get().getLayer("game_area"); }
Layer &BaseView::toolsLayer() { return Application::get().getLayer("edit_tools"); }

void BaseView::onAttach() {}

void BaseView::onUpdate() {}

void BaseView::onDraw() {}

void BaseView::onDetach() {}

void BaseView::onMouseEvent(MouseEvent &) {}

bool BaseView::handleSelection(Entity) { return false; }

void BaseView::show() { m_isVisible = true; }

void BaseView::hide() { m_isVisible = false; }

bool BaseView::isVisible() { return m_isVisible; }

} // namespace Engine
