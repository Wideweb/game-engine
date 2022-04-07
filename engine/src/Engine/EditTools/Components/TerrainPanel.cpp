#include "TerrainPanel.hpp"

#include "Application.hpp"
#include "ImGuiWidgets.hpp"
#include "Math.hpp"
#include "ModelFactory.hpp"
#include "Render3DComponent.hpp"
#include "TerrainCollisionComponent.hpp"

#include "cmath"
#include "imgui/imgui.h"

namespace Engine {

TerrainPanel::TerrainPanel(GameObjectModel &model) : m_Model(model) {}

void TerrainPanel::onAttach() {
    m_Model.entityChange$.addEventCallback([this](Entity entity) {
        if (m_Model.hasTerrainCollision()) {
            auto &terrain = gameLayer().getCoordinator().GetComponent<TerrainCollisionComponent>(m_Model.entity());
            m_Columns = terrain.columns;
            m_Rows = terrain.rows;
        }
    });
}

void TerrainPanel::onUpdate() {
    auto &coordinator = gameLayer().getCoordinator();
    auto &terrain = coordinator.GetComponent<TerrainCollisionComponent>(m_Model.entity());
    if (m_Columns != terrain.columns || m_Rows != terrain.rows) {
        auto &render = coordinator.GetComponent<Render3DComponent>(m_Model.entity());

        const auto &model = Application::get().getModels().GetModel<InstancedModel>(render.model);
        const auto &oldVertices = model->meshes[0].vertices;

        auto newModel = ModelFactory::createPlane(1.0f, m_Columns, m_Rows);
        auto newVertices = newModel->meshes[0].vertices;
        auto newIndices = newModel->meshes[0].indices;

        float columnScale = static_cast<float>(terrain.columns) / m_Columns;
        float rowScale = static_cast<float>(terrain.rows) / m_Rows;

        int columnPadding = std::ceil(columnScale / 2.0f);
        int rowPadding = std::ceil(rowScale / 2.0f);

        for (int i = 0; i <= m_Rows; i++) {
            float y = static_cast<float>(i) * rowScale;

            int yi = static_cast<int>(std::floor(y));
            float offsetY = y - yi;

            int rowPaddingBegin = -rowPadding;
            if (!Math::isEqual(offsetY, 0.0f)) {
                rowPaddingBegin++;
            }

            for (int j = 0; j <= m_Columns; j++) {
                float x = static_cast<float>(j) * columnScale;

                int xi = static_cast<int>(std::floor(x));
                float offsetX = x - xi;

                int columnPaddingBegin = -columnPadding;
                if (!Math::isEqual(offsetX, 0.0f)) {
                    columnPaddingBegin++;
                }

                int newIndex = i * (m_Columns + 1) + j;

                if (Math::isEqual(offsetY, 0.0f) && Math::isEqual(offsetX, 0.0f)) {
                    newVertices[newIndex] = oldVertices[yi * (terrain.columns + 1) + xi];
                    continue;
                }

                Mesh::Vertex vertex;
                int div = 0;
                for (int dy = rowPaddingBegin; dy <= rowPadding; dy++) {
                    for (int dx = columnPaddingBegin; dx <= columnPadding; dx++) {
                        if (yi + dy < 0 || yi + dy > terrain.rows) {
                            continue;
                        }

                        if (xi + dx < 0 || xi + dx > terrain.columns) {
                            continue;
                        }

                        if ((i == 0 || i == m_Rows) && dy != 0) {
                            continue;
                        }

                        if ((j == 0 || j == m_Columns) && dx != 0) {
                            continue;
                        }

                        int index = (yi + dy) * (terrain.columns + 1) + (xi + dx);
                        float impact = 1.0f;

                        if (dy == rowPaddingBegin && !Math::isEqual(offsetY, 0.0f)) {
                            impact *= (1.0f - offsetY);
                        }

                        if (dx == columnPaddingBegin && !Math::isEqual(offsetX, 0.0f)) {
                            impact *= (1.0f - offsetX);
                        }

                        if (dy == rowPadding && !Math::isEqual(offsetY, 0.0f)) {
                            impact *= offsetY;
                            div--;
                        }

                        if (dx == columnPadding && !Math::isEqual(offsetX, 0.0f)) {
                            impact *= offsetX;
                            div--;
                        }

                        vertex += oldVertices[index] * impact;
                        div++;
                    }
                }

                newVertices[newIndex] = vertex / div;
            }
        }

        m_Model.updateTerrain(newVertices, newIndices, m_Columns, m_Rows);
    }
}

void TerrainPanel::onDraw(int x, int y) {
    static bool expanded = false;
    ImGuiWidgets::Collapse("Terrain", expanded);
    if (!expanded) {
        return;
    }

    ImGui::PushItemWidth(120.0f);
    float padding = 10.0f;

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("columns", &m_Columns, 1, 10);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputInt("rows", &m_Rows, 1, 10);
    }

    {
        ImGuiWidgets::PaddingLeft(padding);
        ImGui::Text("Brush: ");

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("radius", &brushRadius, 0.1f, 0.01f);

        ImGuiWidgets::PaddingLeft(padding);
        ImGui::InputFloat("strength", &brushStrength, 0.01f, 0.001f);
    }

    if (m_Columns <= 0) {
        m_Columns = 1;
    }

    if (m_Rows <= 0) {
        m_Rows = 1;
    }

    ImGui::PopItemWidth();
    ImGui::NewLine();
}

} // namespace Engine
