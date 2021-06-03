#pragma once

#include "Entity.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include "Skelet.hpp"

namespace Engine {

struct SkeletJointState {
    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 scale;
};

class SkeletState {
  private:
    std::vector<SkeletJointState> m_Snapshot;

    std::string m_CurrentAnimation;
    std::string m_NextAnimation;

    bool m_Transitioning = false;
    double m_Elapsed = 0;

  public:
    float mixDuration = 0.5f;

    void play(std::string animation) {
        if (m_CurrentAnimation.empty()) {
            m_CurrentAnimation = animation;
            m_NextAnimation = animation;
            return;
        }

        if (m_NextAnimation != animation) {
            m_Elapsed = 0.0;
            m_NextAnimation = animation;
            m_Transitioning = true;
        }
    }

    std::vector<glm::mat4> update(Skelet &skelet, double elapsed) {
        m_Elapsed += elapsed;

        if (m_Transitioning) {
            auto m_NextAnimationStart = getAnimationTransform(skelet, m_NextAnimation, m_Elapsed);
            m_Snapshot = getTransitionTransform(m_Snapshot, m_NextAnimationStart, m_Elapsed);

            if (m_Elapsed > mixDuration) {
                m_Transitioning = false;
                m_CurrentAnimation = m_NextAnimation;
            }

        } else {
            m_Snapshot = getAnimationTransform(skelet, m_CurrentAnimation, m_Elapsed);
        }

        std::vector<glm::mat4> transforms;
        transforms.resize(skelet.joints.size());
        for (size_t i = 0; i < skelet.joints.size(); i++) {
            auto &joint = skelet.joints[i];

            glm::mat4 positionMatrix = glm::translate(m_Snapshot[i].position);
            glm::mat4 rotationMatrix = glm::toMat4(m_Snapshot[i].rotation);
            glm::mat4 scaleMatrix = glm::scale(m_Snapshot[i].scale);

            glm::mat4 transform = positionMatrix * rotationMatrix * scaleMatrix;

            if (joint.parentId > 0) {
                auto &parent = skelet.joints[static_cast<unsigned int>(joint.parentId)];
                joint.globalTransformation = parent.globalTransformation * transform;

                transforms[i] = joint.globalTransformation * joint.offset;
            } else {
                joint.globalTransformation = transform;
                transforms[i] = joint.globalTransformation * joint.offset;
            }
        }

        return transforms;
    }

  private:
    std::vector<SkeletJointState> getTransitionTransform(const std::vector<SkeletJointState> &from,
                                                         const std::vector<SkeletJointState> &to, double elapsed) {
        std::vector<SkeletJointState> transforms;
        transforms.resize(from.size());

        float factor = glm::clamp(static_cast<float>(elapsed / mixDuration), 0.0f, 1.0f);

        for (size_t i = 0; i < from.size(); i++) {
            transforms[i].position = glm::mix(from[i].position, to[i].position, factor);
            transforms[i].rotation = glm::slerp(from[i].rotation, to[i].rotation, factor);
            transforms[i].scale = glm::mix(from[i].scale, to[i].scale, factor);
        }

        return transforms;
    }

    std::vector<SkeletJointState> getAnimationTransform(Skelet &skelet, std::string animationId, double elapsed) {
        std::vector<SkeletJointState> transforms;
        transforms.resize(skelet.joints.size());

        auto &animation = skelet.animations[animationId];

        for (size_t i = 0; i < skelet.joints.size(); i++) {
            auto &frames = animation.joints[i].keyFrames;

            if (frames.empty()) {
                transforms[i].position = glm::vec3(0.0f);
                transforms[i].rotation = glm::quat(glm::vec3(0.0f));
                transforms[i].scale = glm::vec3(0.0f);

                continue;
            }

            double timeInTicks = elapsed * animation.ticksPerSecond;
            double animationTime = fmod(timeInTicks, animation.duration);

            size_t frame = getFrame(frames, animationTime);
            size_t prevFrame = frame - 1;

            double deltaTime = frames[frame].timeStamp - frames[prevFrame].timeStamp;
            float factor = static_cast<float>((animationTime - frames[prevFrame].timeStamp) / deltaTime);

            transforms[i].position = glm::mix(frames[prevFrame].position, frames[frame].position, factor);
            transforms[i].rotation = glm::slerp(frames[prevFrame].rotation, frames[frame].rotation, factor);
            transforms[i].scale = glm::mix(frames[prevFrame].scale, frames[frame].scale, factor);
        }

        return transforms;
    }

    size_t getFrame(const std::vector<JointAnimationKeyFrame> &frames, double time) const {
        for (size_t i = 1; i < frames.size(); i++) {
            if (time < frames[i].timeStamp) {
                return i;
            }
        }

        return 1;
    }
};

class SkeletComponent {
  public:
    SkeletState state;

    SkeletComponent() {}
    SkeletComponent(std::string animation) { state.play(animation); }
};

} // namespace Engine