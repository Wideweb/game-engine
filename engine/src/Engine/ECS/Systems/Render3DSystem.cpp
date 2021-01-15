#include "Render3DSystem.hpp"

#include "Application.hpp"
#include "LocationComponent.hpp"
#include "ModelInstanceManager.hpp"
#include "Render3DComponent.hpp"
#include "cmath"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

void Render3DSystem::Update(ComponentManager &components) const {
    auto &scene = Application::get().getScene();

    for (auto entity : m_Entities) {
        auto &render = components.GetComponent<Render3DComponent>(entity);
        auto &location = components.GetComponent<LocationComponent>(entity);

        if (render.instance != NoModelInstance && !location.updated) {
            continue;
        }

        glm::mat4 model(1);
        model = glm::translate(model, location.position);
        model = glm::rotate(model, location.rotation.x,
                            glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, location.rotation.y,
                            glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, location.rotation.z,
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(render.scale));

        if (render.instance == NoModelInstance) {
            render.instance = scene.addObject(render.model, model);
        } else if (location.updated) {
            scene.updateObject(render.model, model, render.instance);
            location.updated = false;
        }
    }
}

} // namespace Engine
