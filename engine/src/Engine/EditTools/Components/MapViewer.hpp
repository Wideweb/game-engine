#pragma once

#include "BaseView.hpp"
#include "Entity.hpp"
#include "Texture.hpp"

#include <memory>
#include <vector>
#include <iostream>

namespace Engine {

class MapViewer : public BaseView {
  private:
    Entity m_Map;
    Texture m_ColorBuffer;
    std::vector<uint8_t> m_ColorBufferData;

  public:
    MapViewer();

    void onAttach() override;
    void onDraw() override;
    bool handleSelection(Entity entity) override;

    void show() override;
    void hide() override;
};

} // namespace Engine
