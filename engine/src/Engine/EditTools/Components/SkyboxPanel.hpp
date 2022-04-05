#pragma once

#include "BaseView.hpp"

#include <glm/vec3.hpp>

namespace Engine {

class SkyboxPanel : public BaseView {
  public:
    using BaseView::BaseView;

    void onDraw(int x, int y) override;

  private:
    void showFace(size_t index);
};

} // namespace Engine
