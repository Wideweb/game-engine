#pragma once

#include "BaseView.hpp"
#include "RenderSettings.hpp"

namespace Engine {

class RenderPanel : public BaseView {
  public:
    void onDraw(int x, int y) override;
};

} // namespace Engine
