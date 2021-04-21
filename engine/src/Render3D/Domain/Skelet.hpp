#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct JointAnimationKeyFrame {
    glm::quat rotation;
    glm::vec3 position;
    float timeStamp;
};

struct JointAnimation {
    std::vector<JointAnimationKeyFrame> keyFrames;
};

struct Joint {
    glm::mat4 offset;
    glm::mat4 transform;
    glm::mat4 globalTransformation;
    std::string name;
    int parentId;
};

struct SkeletAnimation {
    float ticksPerSecond;
    float duration;
    std::vector<JointAnimation> joints;
};

struct Skelet {
    std::vector<Joint> joints;
    std::unordered_map<std::string, SkeletAnimation> animations;
};