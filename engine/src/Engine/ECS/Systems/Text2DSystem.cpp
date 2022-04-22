#include "Text2DSystem.hpp"

#include "Application.hpp"
#include "Location2DComponent.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/vec2.hpp>

namespace Engine {

void Text2DSystem::OnRemoveComponent(Entity entity) const {
    auto &text2D = getCoordinator().GetComponent<Text2DComponent>(entity);

    if (!text2D.instanced) {
        return;
    }

    getScene().removeText(entity);
    text2D.instanced = false;
}

void Text2DSystem::Attach(ComponentManager &components) const {
    const auto cmpArray = components.GetComponentArray<Text2DComponent>();
    cmpArray->beforeRemove$.addEventCallback([this](Entity entity) { OnRemoveComponent(entity); });
    cmpArray->active$.addEventCallback([this](Entity entity, bool isActive) {
        if (!isActive) {
            OnRemoveComponent(entity);
        }
    });
}

void Text2DSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();
    auto &fonts = Application::get().getFonts();

    for (const auto entity : m_Entities) {
        auto &location = components.GetComponent<Location2DComponent>(entity);
        auto &textCmp = components.GetComponent<Text2DComponent>(entity);

        if (textCmp.font.empty()) {
            if (textCmp.instanced) {
                scene.removeText(entity);
            }

            textCmp.instanced = false;
            textCmp.updated = false;
            continue;
        }

        if (!textCmp.instanced) {
            auto font = fonts.Get(textCmp.font);
            auto alignment = getAlignment(textCmp, *font);

            auto ndcTransform = Location2DComponent::getFullTransform(entity, components, true);
            auto modelTransform = Location2DComponent::getFullTransform(entity, components) * alignment;

            scene.addText(entity, textCmp.text, fonts.Get(textCmp.font), modelTransform, ndcTransform, textCmp.color);
            textCmp.instanced = true;
        } else if (textCmp.updated || location.isUpdated(entity, components)) {
            auto font = fonts.Get(textCmp.font);
            auto alignment = getAlignment(textCmp, *font);

            auto ndcTransform = Location2DComponent::getFullTransform(entity, components, true);
            auto modelTransform = Location2DComponent::getFullTransform(entity, components) * alignment;

            scene.updateText(entity, textCmp.text, fonts.Get(textCmp.font), modelTransform, ndcTransform,
                             textCmp.color);
            textCmp.updated = false;
        }
    }
}

glm::mat4 Text2DSystem::getAlignment(Text2DComponent &text, const Font &font) const {
    float textWidth = getTextWidth(text.text, font);
    auto horizontalAlignment = glm::mat4(1.0);
    switch (text.horizontalAlignment) {
    case Text2DComponent::HorizontalAlignment::Left:
        horizontalAlignment = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
        break;
    case Text2DComponent::HorizontalAlignment::Middle:
        horizontalAlignment = glm::translate(glm::mat4(1.0), glm::vec3(-textWidth / 2.0f, 0.0f, 0.0f));
        break;
    case Text2DComponent::HorizontalAlignment::Right:
        horizontalAlignment = glm::translate(glm::mat4(1.0), glm::vec3(-textWidth, 0.0f, 0.0f));
        break;
    }

    float textHeight = getTextHeight(text.text, font);
    auto verticalAlignment = glm::mat4(1.0);
    switch (text.verticalAlignment) {
    case Text2DComponent::VerticalAlignment::Bottom:
        verticalAlignment = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
        break;
    case Text2DComponent::VerticalAlignment::Middle:
        verticalAlignment = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -textHeight / 2.0f, 0.0f));
        break;
    case Text2DComponent::VerticalAlignment::Top:
        verticalAlignment = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -textHeight, 0.0f));
        break;
    }

    return horizontalAlignment * verticalAlignment;
}

float Text2DSystem::getTextWidth(const std::string &text, const Font &font) const {
    float width = 0.0f;

    std::string::const_iterator i;
    for (i = text.begin(); i != text.end(); i++) {
        char code = *i;
        if (font.characters.find(code) == font.characters.end()) {
            code = '?';
        }

        width += font.characters.at(code).advance >> 6;
    }

    return width;
}

float Text2DSystem::getTextHeight(const std::string &text, const Font &font) const {
    float maxHeight = 0.0f;

    std::string::const_iterator i;
    for (i = text.begin(); i != text.end(); i++) {
        char code = *i;
        if (font.characters.find(code) == font.characters.end()) {
            continue;
        }

        float height = font.characters.at(code).size.y;
        maxHeight = maxHeight < height ? height : maxHeight;
    }

    return maxHeight;
}

} // namespace Engine
