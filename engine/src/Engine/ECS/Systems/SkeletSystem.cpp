#include "SkeletSystem.hpp"

#include "Application.hpp"
#include "Render3DComponent.hpp"
#include "SkeletComponent.hpp"
#include "SkinnedModel.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>

namespace Engine {

void SkeletSystem::Update(ComponentManager &components) const {
    auto &models = getModels();
    auto &scene = getScene();

    for (const auto entity : m_Entities) {
        auto &skeletCmp = components.GetComponent<SkeletComponent>(entity);
        auto &renderCmp = components.GetComponent<Render3DComponent>(entity);

        auto model = models.GetModel<SkinnedModel>(renderCmp.model);
        auto &skelet = model->skelet;
        auto &animation = skelet.animations[skeletCmp.animation];

        std::vector<glm::mat4> transforms;
        transforms.resize(skelet.joints.size());

        for (size_t i = 0; i < skelet.joints.size(); i++) {
            auto &joint = skelet.joints[i];
            auto &frames = animation.joints[i].keyFrames;

            if (frames.empty()) {
                glm::mat4 transform = glm::mat4(1);

                if (joint.parentId > 0) {
                    auto &parent =
                        skelet
                            .joints[static_cast<unsigned int>(joint.parentId)];
                    joint.globalTransformation =
                        parent.globalTransformation * transform;

                    transforms[i] = joint.globalTransformation * joint.offset;
                } else {
                    joint.globalTransformation = transform;
                    transforms[i] = joint.globalTransformation * joint.offset;
                }

                continue;
            }

            float timeInTicks = Application::get().getTime().getTotalSeconds() *
                                animation.ticksPerSecond;
            float animationTime = fmod(timeInTicks, animation.duration);

            size_t frame = getFrame(frames, animationTime);
            size_t prevFrame = frame - 1;

            float deltaTime =
                frames[frame].timeStamp - frames[prevFrame].timeStamp;
            float factor =
                (animationTime - frames[prevFrame].timeStamp) / deltaTime;

            glm::vec3 position = glm::mix(frames[prevFrame].position,
                                          frames[frame].position, factor);

            glm::quat rotation = glm::slerp(frames[prevFrame].rotation,
                                            frames[frame].rotation, factor);

            glm::vec3 scale =
                glm::mix(frames[prevFrame].scale, frames[frame].scale, factor);

            glm::mat4 positionMatrix = glm::translate(position);
            glm::mat4 rotationMatrix = glm::toMat4(rotation);
            glm::mat4 scaleMatrix = glm::scale(scale);

            glm::mat4 transform = positionMatrix * rotationMatrix * scaleMatrix;

            if (joint.parentId > 0) {
                auto &parent =
                    skelet.joints[static_cast<unsigned int>(joint.parentId)];
                joint.globalTransformation =
                    parent.globalTransformation * transform;

                transforms[i] = joint.globalTransformation * joint.offset;
            } else {
                joint.globalTransformation = transform;
                transforms[i] = joint.globalTransformation * joint.offset;
            }
        }

        scene.updateObject(renderCmp.model, transforms, renderCmp.instance);
    }
}

size_t SkeletSystem::getFrame(const std::vector<JointAnimationKeyFrame> &frames,
                              float time) const {
    for (size_t i = 1; i < frames.size(); i++) {
        if (time < frames[i].timeStamp) {
            return i;
        }
    }

    return 1;
}

} // namespace Engine
