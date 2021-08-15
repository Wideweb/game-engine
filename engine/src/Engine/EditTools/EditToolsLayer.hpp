#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

#include "BehaviourPanel.hpp"
#include "CameraDirector.hpp"
#include "ContentBrowserPanel.hpp"
#include "DirectedLightModel.hpp"
#include "DirectedLightPanel.hpp"
#include "GameObjectModel.hpp"
#include "ImguiImpl.hpp"
#include "MaterialPanel.hpp"
#include "ParticlesPanel.hpp"
#include "SceneHierarchyPanel.hpp"
#include "TransformControls.hpp"
#include "TransformPanel.hpp"
#include "Window.hpp"
#include "GamePanel.hpp"

namespace Engine {

class EditToolsLayer : public Layer {
  private:
    GameObjectModel m_GameObject;
    DirectedLightModel m_DirectedLightModel;
    glm::vec2 m_MousePos;
    std::string m_Logs;

    ImguiImpl m_Imgui;

    std::unique_ptr<TransformControls> m_TransformControls;
    std::unique_ptr<TransformPanel> m_TransformPanel;
    std::unique_ptr<DirectedLightPanel> m_DirectedLightPanel;
    std::unique_ptr<ParticlesPanel> m_ParticlesPanel;
    std::unique_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
    std::unique_ptr<ContentBrowserPanel> m_ContentBrowserPanel;
    std::unique_ptr<MaterialPanel> m_MaterialPanel;
    std::unique_ptr<BehaviourPanel> m_BehaviourPanel;
    std::unique_ptr<CameraDirector> m_CameraDirector;
    std::unique_ptr<GamePanel> m_GamePanel;

    Layer &gameLayer();

  public:
    using Layer::Layer;

    virtual void onAttach() override;
    virtual void onUpdate() override;
    virtual void onDraw() override;
    virtual void onDetach() override;
    virtual void onMouseEvent(MouseEvent &event) override;

    void handleSelection();
};

} // namespace Engine
