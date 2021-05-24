#include "MasterRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"

namespace Engine {

MasterRenderer::MasterRenderer(int width, int height) : m_Viewport{width, height} {
    auto vertexSrc = File::read("./shaders/direct-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/direct-fragment-shader.glsl");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_QuadRenderer = std::make_unique<QuadRenderer>();
    m_ModelRenderer = std::make_unique<ModelRenderer>();
    m_SkyboxRenderer = std::make_unique<SkyboxRenderer>();
    m_GRenderer = std::make_unique<GRenderer>(m_Viewport, *m_ModelRenderer, *m_SkyboxRenderer);
    m_DirectedLightRenderer = std::make_unique<DirectedLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_SpotLightRenderer = std::make_unique<SpotLightRenderer>(m_Viewport, *m_ModelRenderer);
    m_WaterRenderer = std::make_unique<WaterRenderer>(*m_GRenderer);
    m_FlareRenderer = std::make_unique<FlareRenderer>(m_Viewport, *m_QuadRenderer);
}

void MasterRenderer::draw(Camera &camera, Scene &scene, const ModelManager &models) {
    m_Shader->bind();
    m_Shader->setFloat3("u_viewPos", camera.positionVec());
    m_Shader->setMatrix4("u_view", camera.viewMatrix());
    m_Shader->setMatrix4("u_projection", camera.projectionMatrix());

    m_Shader->setInt("u_hasDirectedLight", 0);
    m_Shader->setInt("u_spotLightsNumber", 0);

    if (scene.hasDirectedLight()) {
        m_Shader->setInt("u_hasDirectedLight", 1);
        m_DirectedLightRenderer->apply(camera, scene.getDirectedLight(), *m_Shader, scene, models);
    }

    for (const auto &obj : scene.getSpotLights()) {
        m_SpotLightRenderer->apply(obj.light, obj.position, *m_Shader, scene, models);
    }

    m_ModelRenderer->draw(*m_Shader, scene, models, 1);
    m_SkyboxRenderer->draw(camera, scene);

    m_WaterRenderer->draw(camera, scene, models);
}

void MasterRenderer::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

void MasterRenderer::clear() {
    m_SpotLightRenderer->clear();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace Engine