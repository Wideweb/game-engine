#pragma once

#include "imgui/imgui.h"

namespace Engine {

class ImguiImpl {
  public:
    void OnAttach();
    void OnDetach();
    void Begin();
    void End();
};

} // namespace Engine
