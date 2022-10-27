#include "ModelRenderer.hpp"

#include "glad/glad.h"

namespace Engine {

ModelRenderer::ModelRenderer() {}

void ModelRenderer::draw(Scene &scene, const ModelManager &models, Material* baseMaterial, Shader* activeShader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& materials = scene.getMaterials();
    auto& modelsInstances = scene.getModelsInstances();

    for (size_t i = 0; i < materials.size(); i++) {
        Material* material = materials[i];
        auto& materialInstances = modelsInstances[i];

        if (activeShader != nullptr) {
            material->bind();
            baseMaterial->apply();
            material->apply();

            if (material->getDepthTest()) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
        }

        for (size_t j = 0; j < materialInstances.size(); j++) {
            auto& modelId = materialInstances.keys()[j];
            const auto &model = models.GetModel(modelId);
            ModelInstanceBatch* batch = materialInstances.values()[j].get();
            model->draw(batch, activeShader);
        }
    }
    
    glDisable(GL_BLEND);
}

} // namespace Engine