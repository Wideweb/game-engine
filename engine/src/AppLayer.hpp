#pragma once

#include "Engine.hpp"

#include <filesystem>

class AppLayer : public Engine::Layer {
  private:
    std::filesystem::file_time_type m_ScriptLastWriteTime;

  public:
    using Layer::Layer;

    virtual void onAttach() override;
    virtual void onUpdate() override;
    virtual void onDetach() override;
    virtual void onMouseEvent(Engine::MouseEvent &event) override;
};
