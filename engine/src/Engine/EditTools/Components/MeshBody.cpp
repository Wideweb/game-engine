#include "MeshBody.hpp"

#include "Application.hpp"
#include "EditToolComponent.hpp"
#include "File.hpp"
#include "LocationComponent.hpp"
#include "ParentComponent.hpp"
#include "Render3DComponent.hpp"

#include "imgui/imgui.h"

namespace Engine {

MeshBody::MeshBody(GameObjectModel &model) : m_Model(model) {}

void MeshBody::onAttach() {
    auto &coordinator = gameLayer().getCoordinator();
    auto& materials = Application::get().getMaterials();

    m_MeshBody = coordinator.CreateEntity("mesh-body");
    coordinator.AddComponent(m_MeshBody, LocationComponent());
    coordinator.AddComponent(m_MeshBody, EditToolComponent());
    coordinator.AddComponent(m_MeshBody, ParentComponent(c_NoEntity, false));

    auto render = Render3DComponent("", materials.meshMaterial.get());
    coordinator.AddComponent(m_MeshBody, render);

    hide();
}

void MeshBody::onUpdate() {
    auto &coordinator = gameLayer().getCoordinator();

    if (m_Model.hasRender() && !coordinator.HasComponent<EditToolComponent>(m_Model.entity())) {
        auto render = coordinator.GetComponent<Render3DComponent>(m_MeshBody);
        render.model = coordinator.GetComponent<Render3DComponent>(m_Model.entity()).model;
        render.updated = true;
        coordinator.GetComponent<ParentComponent>(m_MeshBody).setEntity(m_Model.entity());
    } else {
        hide();
    }

    const auto &camera = Application::get().getCamera();
}

void MeshBody::show() {
    auto &coordinator = gameLayer().getCoordinator();

    coordinator.SetComponentActive<Render3DComponent>(m_MeshBody, true);
    coordinator.SetComponentActive<ParentComponent>(m_MeshBody, true);

    BaseView::show();
}

void MeshBody::hide() {
    auto &coordinator = gameLayer().getCoordinator();

    coordinator.SetComponentActive<Render3DComponent>(m_MeshBody, false);
    coordinator.SetComponentActive<ParentComponent>(m_MeshBody, false);

    BaseView::hide();
}

} // namespace Engine
