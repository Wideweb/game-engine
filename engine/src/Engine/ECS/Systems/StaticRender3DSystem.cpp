#include "StaticRender3DSystem.hpp"

#include "Application.hpp"
#include "LocationComponent.hpp"
#include "StaticRender3DComponent.hpp"
#include "cmath"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

void StaticRender3DSystem::Update(ComponentManager &components) const {
    auto &scene = getScene();
    auto &coordinator = getCoordinator();

    std::vector<Entity> entitis(m_Entities.begin(), m_Entities.end());

    for (auto entity : entitis) {
        auto &render = components.GetComponent<StaticRender3DComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        glm::mat4 model(1);
        model = glm::translate(model, location.position);
        model = model * glm::toMat4(glm::quat(location.rotation));
        model = glm::scale(model, glm::vec3(render.scale));

        scene.addStaticObject(entity, render.model, model);
        coordinator.RemoveComponent<StaticRender3DComponent>(entity);
    }
}

} // namespace Engine
