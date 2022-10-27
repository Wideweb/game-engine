#include "DeferredRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"

#include <iostream>

namespace Engine {

DeferredRenderer::DeferredRenderer(DirectedLightRenderer &directedLightRenderer, QuadRenderer &quadRenderer)
    : m_DirectedLightRenderer(directedLightRenderer), m_QuadRenderer(quadRenderer) {
    auto vertexSrc = File::read("./shaders/deferred-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/deferred-fragment-shader.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);
    m_Material.setShader(&m_Shader);
}

DeferredRenderer::~DeferredRenderer() { m_Shader.free(); }

void DeferredRenderer::draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                            Texture &ssaoMap, Camera &camera, Scene &scene, const ModelManager &models,
                            RenderSettings &settings, RendererState &state) {
    m_Shader.bind();
    m_Shader.setInt("u_hasSSAO", 1);
    m_Shader.setTexture("u_ssaoMap", ssaoMap);
    doDraw(colorMap, positionMap, normalMap, specularMap, camera, scene, models, settings, state);
}

void DeferredRenderer::draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                            Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings,
                            RendererState &state) {
    m_Shader.bind();
    m_Shader.setInt("u_hasSSAO", 0);
    doDraw(colorMap, positionMap, normalMap, specularMap, camera, scene, models, settings, state);
}

void DeferredRenderer::doDraw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                              Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings,
                              RendererState &state) {
    m_Shader.setFloat3("u_viewPos", camera.positionVec());
    m_Shader.setMatrix4("u_view", camera.viewMatrix());
    m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader.setFloat("u_threshold", settings.threshold);

    m_Shader.setInt("u_fog", settings.fog);
    m_Shader.setFloat3("u_fogColor", settings.fogColor);
    m_Shader.setFloat("u_density", settings.fogDensity);
    m_Shader.setFloat("u_gradient", settings.fogGradient);

    m_Shader.setInt("u_hasDirectedLight", 0);
    if (scene.hasDirectedLight()) {
        m_Shader.setInt("u_hasDirectedLight", 1);
        Material* baseMaterial = state.baseMaterial;
        state.baseMaterial = &m_Material;
        m_DirectedLightRenderer.apply(camera, scene, models, state);
        state.baseMaterial = baseMaterial;
        m_Shader.bind();
        m_Material.apply(&m_Shader);
    }

    m_Shader.setTexture("u_colorMap", colorMap);
    m_Shader.setTexture("u_positionMap", positionMap);
    m_Shader.setTexture("u_normalMap", normalMap);
    m_Shader.setTexture("u_specularMap", specularMap);
    // m_Shader->setTexture("u_depthMap", );

    m_QuadRenderer.draw();
}

} // namespace Engine