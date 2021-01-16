#include "StaticRender3DSystem.hpp"

#include "Application.hpp"
#include "LocationComponent.hpp"
#include "ModelInstanceManager.hpp"
#include "StaticRender3DComponent.hpp"
#include "cmath"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
        model = glm::rotate(model, location.rotation.x,
                            glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, location.rotation.y,
                            glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, location.rotation.z,
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(render.scale));

        render.instance = scene.addObject(render.model, model);
        coordinator.RemoveComponent<StaticRender3DComponent>(entity);
    }
}

} // namespace Engine
