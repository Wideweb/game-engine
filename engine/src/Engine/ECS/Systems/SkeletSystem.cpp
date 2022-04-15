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

        if (renderCmp.model.empty() || !models.Is<SkinnedModel>(renderCmp.model)) {
            continue;
        }

        auto model = models.GetModel<SkinnedModel>(renderCmp.model);
        auto &skelet = model->skelet;

        std::vector<glm::mat4> transforms =
            skeletCmp.state.update(skelet, Application::get().getTime().getDeltaSeconds());

        scene.updateObject(entity, renderCmp.model, transforms);
    }
}

size_t SkeletSystem::getFrame(const std::vector<JointAnimationKeyFrame> &frames, double time) const {
    for (size_t i = 1; i < frames.size(); i++) {
        if (time < frames[i].timeStamp) {
            return i;
        }
    }

    return 1;
}

} // namespace Engine
