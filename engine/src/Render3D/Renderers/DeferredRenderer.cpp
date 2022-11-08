#include "DeferredRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"

#include <iostream>

namespace Engine {

DeferredRenderer::DeferredRenderer(DirectedLightRenderer &directedLightRenderer, SpotLightRenderer &spotLightRenderer, QuadRenderer &quadRenderer)
    : m_DirectedLightRenderer(directedLightRenderer), m_SpotLightRenderer(spotLightRenderer), m_QuadRenderer(quadRenderer) {
    auto vertexSrc = File::readGLSL("./shaders/pass/common/clipping-space-textured.vertex.glsl");
    auto fragmentSrc = File::readGLSL("./shaders/pass/deferred.fragment.glsl");
    m_Shader = Shader(vertexSrc, fragmentSrc);
    m_Material.setShader(&m_Shader);
}

DeferredRenderer::~DeferredRenderer() { m_Shader.free(); }

void DeferredRenderer::draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                            Texture &ssaoMap, Camera &camera, Scene &scene, const ModelManager &models,
                            RenderSettings &settings, RendererState &state) {
    m_Material.setInt("u_hasSSAO", 1);
    m_Material.setTexture("u_ssaoMap", &ssaoMap);
    doDraw(colorMap, positionMap, normalMap, specularMap, camera, scene, models, settings, state);
}

void DeferredRenderer::draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                            Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings,
                            RendererState &state) {
    m_Material.setInt("u_hasSSAO", 0);
    doDraw(colorMap, positionMap, normalMap, specularMap, camera, scene, models, settings, state);
}

void DeferredRenderer::doDraw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                              Camera &camera, Scene &scene, const ModelManager &models, RenderSettings &settings,
                              RendererState &state) {
    Material* baseMaterial = state.baseMaterial;
    state.baseMaterial = &m_Material;

    m_Material.setFloat3("u_viewPos", camera.positionVec());
    m_Material.setMatrix4("u_view", camera.viewMatrix());
    m_Material.setMatrix4("u_projection", camera.projectionMatrix());
    m_Material.setFloat("u_threshold", settings.threshold);

    m_Material.setInt("u_fog", settings.fog);
    m_Material.setFloat3("u_fogColor", settings.fogColor);
    m_Material.setFloat("u_density", settings.fogDensity);
    m_Material.setFloat("u_gradient", settings.fogGradient);

    m_Material.setInt("u_directedLightsNumber", 0);
    m_Material.setInt("u_spotLightsNumber", 0);

    if (scene.hasDirectedLight()) {
        m_DirectedLightRenderer.apply(camera, scene, models, state);
    }

    for (const auto &obj : scene.getSpotLights()) {
        m_SpotLightRenderer.apply(obj.light, obj.position, scene, models, state);
    }

    m_Material.setTexture("u_colorMap", &colorMap);
    m_Material.setTexture("u_positionMap", &positionMap);
    m_Material.setTexture("u_normalMap", &normalMap);
    m_Material.setTexture("u_specularMap", &specularMap);
    // m_Shader->setTexture("u_depthMap", );

    m_Material.bind();
    m_Material.apply();

    m_QuadRenderer.draw();

    state.baseMaterial = baseMaterial;
}

} // namespace Engine