#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

#include "BehaviourPanel.hpp"
#include "CameraDirector.hpp"
#include "ContentBrowserPanel.hpp"
#include "DirectedLightDirector.hpp"
#include "DirectedLightPanel.hpp"
#include "GameObjectModel.hpp"
#include "GamePanel.hpp"
#include "ImguiImpl.hpp"
#include "MaterialPanel.hpp"
#include "ParticlesPanel.hpp"
#include "RenderPanel.hpp"
#include "SceneHierarchyPanel.hpp"
#include "SkeletPanel.hpp"
#include "TransformControlsPosition.hpp"
#include "TransformControlsRotation.hpp"
#include "TransformControlsScale.hpp"
#include "TransformPanel.hpp"
#include "VelocityPanel.hpp"
#include "Window.hpp"

namespace Engine {

class EditToolsLayer : public Layer {
  private:
    GameObjectModel m_GameObject;
    glm::vec2 m_MousePos;
    std::string m_Logs;

    ImguiImpl m_Imgui;

    std::unique_ptr<TransformControlsPosition> m_TransformControlsPosition;
    std::unique_ptr<TransformControlsRotation> m_TransformControlsRotation;
    std::unique_ptr<TransformControlsScale> m_TransformControlsScale;
    std::unique_ptr<TransformPanel> m_TransformPanel;
    std::unique_ptr<DirectedLightPanel> m_DirectedLightPanel;
    std::unique_ptr<ParticlesPanel> m_ParticlesPanel;
    std::unique_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
    std::unique_ptr<ContentBrowserPanel> m_ContentBrowserPanel;
    std::unique_ptr<MaterialPanel> m_MaterialPanel;
    std::unique_ptr<BehaviourPanel> m_BehaviourPanel;
    std::unique_ptr<CameraDirector> m_CameraDirector;
    std::unique_ptr<GamePanel> m_GamePanel;
    std::unique_ptr<VelocityPanel> m_VelocityPanel;
    std::unique_ptr<DirectedLightDirector> m_DirectedLightDirector;
    std::unique_ptr<SkeletPanel> m_SkeletPanel;
    std::unique_ptr<RenderPanel> m_RenderPanel;

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
