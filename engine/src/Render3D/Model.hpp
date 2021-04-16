#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "Mesh.hpp"

namespace Engine {

struct BoneAnimationKeyFrame {
    double timeStamp;
    glm::vec3 position;
    glm::quat rotation;
};

struct BoneAnimation {
    std::vector<BoneAnimationKeyFrame> keyFrames;
};

struct Bone {
    std::string name;
    glm::mat4 offset;
    glm::mat4 transform;
    unsigned int parentId;
    bool hasParent;
    glm::mat4 globalTransformation;
};

class Model {
  public:
    std::vector<Mesh> meshes;
    std::vector<Bone> bones;
    std::vector<BoneAnimation> animation;

    unsigned int frame = 0;

    Model();
    Model(const std::vector<Mesh> &meshes);
    ~Model();

    void setUp();
};

} // namespace Engine