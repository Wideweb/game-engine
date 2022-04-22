#pragma once

#include "BaseSystem.hpp"

#include "Text2DComponent.hpp"

#include <glm/mat4x4.hpp>

namespace Engine {

class Text2DSystem : public BaseSystem {
  public:
    using BaseSystem::BaseSystem;

    virtual void Attach(ComponentManager &components) const override;
    virtual void Update(ComponentManager &components) const override;

  private:
    void OnRemoveComponent(Entity entity) const;

    glm::mat4 getAlignment(Text2DComponent &text, const Font &font) const;
    float getTextWidth(const std::string &text, const Font &font) const;
    float getTextHeight(const std::string &text, const Font &font) const;
};

} // namespace Engine
