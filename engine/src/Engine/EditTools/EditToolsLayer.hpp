#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <functional>
#include <memory>

#include "CameraDirector.hpp"
#include "ConsolePanel.hpp"
#include "ContentBrowserPanel.hpp"
#include "DirectedLightDirector.hpp"
#include "EditToolsEvent.hpp"
#include "GameObjectModel.hpp"
#include "GamePanel.hpp"
#include "ImguiImpl.hpp"
#include "InspectorPanel.hpp"
#include "RenderPanel.hpp"
#include "SceneHierarchyPanel.hpp"
#include "ScenePanel.hpp"
#include "ToolbarPanel.hpp"
#include "MapViewer.hpp"
#include "Window.hpp"

namespace Engine {

struct EditToolsView {
    std::unique_ptr<BaseView> view;
    std::function<bool(void)> visible;
};

class EditToolsLayer : public Layer {
  private:
    GameObjectModel m_GameObject;
    glm::vec2 m_MousePos;
    ImguiImpl m_Imgui;
    glm::vec3 m_Pivot;
    EditToolsEventPool m_EventPool;
    std::array<EditToolsView, 3> m_SceneTools;
    std::array<bool, 2> m_SceneToolsVisibility = {false};

    std::unique_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
    std::unique_ptr<ContentBrowserPanel> m_ContentBrowserPanel;
    std::unique_ptr<RenderPanel> m_RenderPanel;
    std::unique_ptr<ScenePanel> m_ScenePanel;
    std::unique_ptr<ToolbarPanel> m_ToolbarPanel;
    std::unique_ptr<GamePanel> m_GamePanel;
    std::unique_ptr<InspectorPanel> m_InspectorPanel;
    std::unique_ptr<ConsolePanel> m_ConsolePanel;

    Layer &gameLayer();

  public:
    using Layer::Layer;

    virtual void onAttach() override;
    virtual void onUpdate() override;
    virtual void onDraw() override;
    virtual void onDetach() override;
    virtual void onMouseEvent(MouseEvent &event) override;

    void handleSelection();
    void onBeforeGameDraw();
    void onAfterGameDraw();
};

} // namespace Engine
