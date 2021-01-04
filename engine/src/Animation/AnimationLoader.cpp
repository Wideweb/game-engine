#include "AnimationLoader.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace Engine {

AnimationScene AnimationLoader::load(const std::string &path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    std::string attribute;

    std::vector<TimeLine> timeLines;

    while (!in.eof() && in >> attribute) {
        if (attribute == "timeLine") {
            int property;

            in >> property;

            timeLines.emplace_back(static_cast<TimeLine::Properties>(property));
        }

        if (attribute == "animation") {
            std::string propertyName;
            float targetValue, time;

            in >> propertyName >> targetValue >> time;

            AnimationProperty property;
            if (propertyName == "x") {
                property = AnimationProperty::X;
            } else if (propertyName == "y") {
                property = AnimationProperty::Y;
            } else if (propertyName == "scale") {
                property = AnimationProperty::Scale;
            } else if (propertyName == "angle") {
                property = AnimationProperty::Angle;
            } else if (propertyName == "alpha") {
                property = AnimationProperty::Alpha;
            } else if (propertyName == "diffuse") {
                property = AnimationProperty::Diffuse;
            } else if (propertyName == "specular") {
                property = AnimationProperty::Specular;
            } else if (propertyName == "ambient") {
                property = AnimationProperty::Ambient;
            }

            timeLines.back().add(
                Action::animation(property, targetValue, time));
        }

        if (attribute == "gap") {
            float time;
            in >> time;
            timeLines.back().add(Action::gap(time));
        }

        if (attribute == "sound") {
            std::string sound;
            float volume;

            in >> sound;
            in >> volume;

            timeLines.back().add(Action::playSound(sound, volume));
        }

        if (attribute == "active") {
            std::string component;
            in >> component;
            timeLines.back().add(Action::active(component));
        }

        if (attribute == "inactive") {
            std::string component;
            in >> component;
            timeLines.back().add(Action::inactive(component));
        }

        in >> std::ws;
    }

    in.close();

    return AnimationScene(timeLines);
}

} // namespace Engine
