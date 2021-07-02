#include "ModelRenderer.hpp"

namespace Engine {

ModelRenderer::ModelRenderer() {}

void ModelRenderer::draw(Shader &shader, Scene &scene, const ModelManager &models, RendererState &state) {
    shader.bind();

    shader.setInt("u_material.diffuse", static_cast<int>(state.activeTextures));
    shader.setInt("u_material.specular", static_cast<int>(state.activeTextures) + 1);
    shader.setInt("u_material.normal", static_cast<int>(state.activeTextures) + 2);

    for (auto &pair : scene.getObjects()) {
        const auto &model = models.GetModel(pair.first);
        auto &instances = pair.second;
        model->draw(shader, instances, state.activeTextures);
    }
}

} // namespace Engine