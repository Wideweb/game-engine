#include "ModelRenderer.hpp"

namespace Engine {

ModelRenderer::ModelRenderer() {}

void ModelRenderer::draw(Shader &shader, Scene &scene, const ModelManager &models, RendererState &state) {
    for (auto &pair : scene.getObjects()) {
        const auto &model = models.GetModel(pair.first);
        auto &instances = pair.second;
        model->draw(shader, instances, state.activeTextures);
    }
}

} // namespace Engine