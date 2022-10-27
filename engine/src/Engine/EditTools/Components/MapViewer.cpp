#include "MapViewer.hpp"

#include "Application.hpp"
#include "Configs.hpp"
#include "File.hpp"
#include "LocationComponent.hpp"
#include "ModelFactory.hpp"
#include "Render3DComponent.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"

namespace Engine {

MapViewer::MapViewer() {}

void MapViewer::onAttach() {
    Map& map = Application::get().getMap();
    Application::get().getModels().RegisterModel(Configs::c_EditToolsModelPrefix + "navigation-map-cell", ModelFactory::createPlane(1, map.width, map.height, false));

    auto &toolsCoordinator = toolsLayer().getCoordinator();
    auto& materials = Application::get().getMaterials();

    auto mapEntity = toolsCoordinator.CreateEntity("NavigationMap");
    auto mapLocation = LocationComponent(map.width / -2.0f, 0.0f, map.height / -2.0f);
    auto mapRender = Render3DComponent(Configs::c_EditToolsModelPrefix + "navigation-map-cell", materials.gridMaterial.get());

    toolsCoordinator.AddComponent(mapEntity, mapLocation);
    toolsCoordinator.AddComponent(mapEntity, mapRender);
    
    m_Map = mapEntity;

    m_ColorBufferData.resize(map.width * map.height * 4);
    for (int i = 0; i < map.width * map.height * 4; i++) {
        m_ColorBufferData[i] = 0x00;
    }
    m_ColorBuffer = Texture::createRGBA8Buffer(map.width, map.height, m_ColorBufferData.data());

    materials.gridMaterial->setTexture("u_colorBuffer", &m_ColorBuffer);
}

void MapViewer::onDraw() { }

bool MapViewer::handleSelection(Entity) {
    return false;
}

void MapViewer::show() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Map, true);
    BaseView::show();
}

void MapViewer::hide() {
    toolsLayer().getCoordinator().SetComponentActive<Render3DComponent>(m_Map, false);
    BaseView::hide();
}

} // namespace Engine
