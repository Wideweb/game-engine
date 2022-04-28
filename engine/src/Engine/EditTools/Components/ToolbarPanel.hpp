#pragma once

#include "BaseView.hpp"
#include "Texture.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

namespace Engine {

class ToolbarPanel : public BaseView {
  private:
    Texture m_IconPlay, m_IconStop;
    glm::vec3 m_CameraPos;
    glm::quat m_CameraRotation;

  public:
    ToolbarPanel();

    void onAttach() override;
    void onDraw() override;
    void onDetach() override;
};

} // namespace Engine
