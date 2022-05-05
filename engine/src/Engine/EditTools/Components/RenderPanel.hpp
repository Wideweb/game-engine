#pragma once

#include "BaseView.hpp"
#include "RenderSettings.hpp"
#include "SkyboxPanel.hpp"

namespace Engine {

class RenderPanel : public BaseView {
  private:
    std::unique_ptr<SkyboxPanel> m_SkyboxPanel;

  public:
    void onAttach() override;
    void onUpdate() override;
    void onDraw() override;
    void onDetach() override;

  private:
    void onDrawGammaCorrectionPanel();
    void onDrawHdrPanel();
    void onDrawSSAOPanel();
    void onDrawFogPanel();
};

} // namespace Engine
