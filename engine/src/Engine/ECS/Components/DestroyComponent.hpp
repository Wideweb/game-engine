#pragma once

namespace Engine {

class DestroyComponent {
  public:
    size_t cycles = 0;

    DestroyComponent() : DestroyComponent(1) {}
    DestroyComponent(size_t cycles) : cycles(cycles) {}
};

} // namespace Engine