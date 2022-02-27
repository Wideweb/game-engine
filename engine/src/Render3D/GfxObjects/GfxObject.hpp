#pragma once

namespace Engine {

using GfxObjectId = unsigned int;

constexpr GfxObjectId c_NoGfxObjectId = 0;

class GfxObject {
  public:
    GfxObjectId id = c_NoGfxObjectId;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    bool empty() { return id == c_NoGfxObjectId; }
    void setEmpty() { id = c_NoGfxObjectId; }
};

} // namespace Engine