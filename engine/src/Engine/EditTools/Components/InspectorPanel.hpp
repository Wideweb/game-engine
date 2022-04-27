#pragma once

#include "Application.hpp"
#include "BaseView.hpp"
#include "GameObjectModel.hpp"

#include <array>
#include <functional>
#include <glm/vec3.hpp>
#include <memory>

namespace Engine {

class IComponentEditTool {
  public:
    enum class Type { Panel = 0, SceneTool = 1 };

    Type type;
    std::shared_ptr<BaseView> view;

    IComponentEditTool(IComponentEditTool::Type type, std::shared_ptr<BaseView> view) : type(type), view(view) {}
    virtual ~IComponentEditTool() = default;
    bool virtual visible() const = 0;
};

template <typename... TComponents> class ComponentEditTool : public IComponentEditTool {
  private:
    GameObjectModel &m_GameObject;

  public:
    ComponentEditTool(IComponentEditTool::Type type, std::shared_ptr<BaseView> view, GameObjectModel &gameObject)
        : IComponentEditTool(type, view), m_GameObject(gameObject) {}

    bool visible() const override {
        if (!m_GameObject.coordinator().template HasAnyComponent<TComponents...>(m_GameObject.entity())) {
            return false;
        }

        if (type == Type::Panel) {
            return true;
        }

        return m_GameObject.coordinator().template IsAnyComponentActive<TComponents...>(m_GameObject.entity()) &&
               Application::get().getTime().poused();
    }
};

class InspectorPanel : public BaseView {
  private:
    GameObjectModel &m_GameObject;
    std::array<std::unique_ptr<IComponentEditTool>, 18> m_Tools;
    std::array<bool, 6> m_SceneToolsVisibility = {false};
    size_t toolsIndex = 0;

    template <typename... TComponents> void addPanel(std::shared_ptr<BaseView> panel) {
        m_Tools[toolsIndex++] =
            std::make_unique<ComponentEditTool<TComponents...>>(IComponentEditTool::Type::Panel, panel, m_GameObject);
    }

    template <typename... TComponents> void addSceneTool(std::shared_ptr<BaseView> tool) {
        m_Tools[toolsIndex++] = std::make_unique<ComponentEditTool<TComponents...>>(IComponentEditTool::Type::SceneTool,
                                                                                    tool, m_GameObject);
    }

  public:
    InspectorPanel(GameObjectModel &model);

    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
    void onBeforeGameDraw() override;
    void onAfterGameDraw() override;
    void onDetach() override;
    void onMouseEvent(MouseEvent &event) override;
    bool handleSelection(Entity entity) override;
};

} // namespace Engine
