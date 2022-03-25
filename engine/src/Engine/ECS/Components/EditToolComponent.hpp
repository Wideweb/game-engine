#pragma once

namespace Engine {

class EditToolComponent {
  public:
    bool canSelect = false;

    EditToolComponent() : EditToolComponent(false) {}
    EditToolComponent(bool canSelect) : canSelect(canSelect) {}
};

} // namespace Engine