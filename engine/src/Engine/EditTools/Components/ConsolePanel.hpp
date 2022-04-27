#pragma once

#include "BaseView.hpp"

#include <string>

namespace Engine {

class ConsolePanel : public BaseView {
  private:
    std::string m_Logs;

  public:
    ConsolePanel();

    void onAttach() override;
    void onDraw() override;
};

} // namespace Engine
