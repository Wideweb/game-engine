#pragma once

#include <string>

namespace Engine {

enum class AnimationProperty {
    None = 0,
    X = 1,
    Y = 2,
    Scale = 3,
    Angle = 4,
    Alpha = 5,
    Diffuse = 6,
    Specular = 7,
    Ambient = 8,
};

enum class ActionType {
    None = 0,
    Animation = 1,
    PlayMusic = 2,
    Active = 3,
    Inactive = 4,
};

struct TimeLineStep {
    ActionType action = ActionType::None;

    // Active, Inactive
    std::string component;

    // Music
    std::string sound;
    float volume;

    // Animation
    AnimationProperty property = AnimationProperty::None;
    float value;

    static TimeLineStep animation(AnimationProperty property, float value) {
        TimeLineStep step;
        step.action = ActionType::Animation;
        step.property = property;
        step.value = value;
        return step;
    }

    static TimeLineStep playSound(const std::string &sound, float volume) {
        TimeLineStep step;
        step.action = ActionType::PlayMusic;
        step.sound = sound;
        step.volume = volume;
        return step;
    }

    static TimeLineStep active(const std::string &component) {
        TimeLineStep step;
        step.action = ActionType::Active;
        step.component = component;
        return step;
    }

    static TimeLineStep inactive(const std::string &component) {
        TimeLineStep step;
        step.action = ActionType::Inactive;
        step.component = component;
        return step;
    }
};

struct Action {
    ActionType type = ActionType::None;

    // Active, Inactive
    std::string component;

    // Music
    std::string sound;
    float volume;

    // Animation
    AnimationProperty property = AnimationProperty::None;
    float time = 0.0;
    float targetValue = 0.0;
    float currentValue = 0.0;

    static Action animation(AnimationProperty property, float targetValue,
                            float time) {
        Action action;
        action.type = ActionType::Animation;
        action.property = property;
        action.targetValue = targetValue;
        action.time = time;
        return action;
    }

    static Action playSound(const std::string &sound, float volume) {
        Action action;
        action.type = ActionType::PlayMusic;
        action.sound = sound;
        action.volume = volume;
        return action;
    }

    static Action active(const std::string &component) {
        Action action;
        action.type = ActionType::Active;
        action.component = component;
        return action;
    }

    static Action inactive(const std::string &component) {
        Action action;
        action.type = ActionType::Inactive;
        action.component = component;
        return action;
    }

    static Action gap(float time) {
        Action action;
        action.type = ActionType::Animation;
        action.property = AnimationProperty::None;
        action.targetValue = 0;
        action.time = time;
        return action;
    }
};

} // namespace Engine