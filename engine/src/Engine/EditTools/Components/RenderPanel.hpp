#pragma once

#include "BaseView.hpp"
#include "RenderSettings.hpp"

namespace Engine {

class RenderPanel : public BaseView {
  private:
    float m_Exposure, m_Gamma, m_Threshold;
    int m_BloomScale, m_Blur;
    ToneMapping m_ToneMapping;

  public:
    void onAttach() override;
    void onUpdate() override;
    void onDraw(int x, int y) override;
};

} // namespace Engine
