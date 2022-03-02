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
}

DeferredRenderer::~DeferredRenderer() { m_Shader.free(); }

void DeferredRenderer::draw(Texture &colorMap, Texture &positionMap, Texture &normalMap, Texture &specularMap,
                            Framebuffer &framebuffer, Camera &camera, Scene &scene, const ModelManager &models,
                            RenderSettings &settings, RendererState &state) {
    RendererState currentRenderState = {.framebuffer = framebuffer};

    m_Shader.bind();
    m_Shader.setFloat3("u_viewPos", camera.positionVec());
    m_Shader.setMatrix4("u_view", camera.viewMatrix());
    m_Shader.setMatrix4("u_projection", camera.projectionMatrix());
    m_Shader.setFloat("u_threshold", settings.threshold);

    m_Shader.setInt("u_hasDirectedLight", 0);
    if (scene.hasDirectedLight()) {
        m_Shader.setInt("u_hasDirectedLight", 1);
        m_DirectedLightRenderer.apply(camera, scene.getDirectedLight(), m_Shader, scene, models, currentRenderState);
    }

    m_Shader.setTexture("u_colorMap", colorMap);
    m_Shader.setTexture("u_positionMap", positionMap);
    m_Shader.setTexture("u_normalMap", normalMap);
    m_Shader.setTexture("u_specularMap", specularMap);
    // m_Shader->setTexture("u_depthMap", );

    m_QuadRenderer.draw();
}

} // namespace Engine