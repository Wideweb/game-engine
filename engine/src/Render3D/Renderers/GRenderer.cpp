#include "GRenderer.hpp"

#include "File.hpp"
#include "TextureLoader.hpp"

#include "glad/glad.h"

#include <iostream>

namespace Engine {

GRenderer::GRenderer(ModelRenderer &modelRenderer, SkyboxRenderer &skyboxRenderer)
    : m_ModelRenderer(modelRenderer), m_SkyboxRenderer(skyboxRenderer) {
    auto vertexSrc = File::read("./shaders/g-buffer-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/g-buffer-fragment-shader.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);
}

void GRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings) {
    m_Shader.bind();
    m_Shader.setMatrix4("u_view", camera.viewMatrix());
    m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader.setFloat4("u_clipPlane", settings.clipPlane);

    m_ModelRenderer.draw(m_Shader, scene, models);
    m_SkyboxRenderer.draw(camera, scene, settings);
}

} // namespace Engine