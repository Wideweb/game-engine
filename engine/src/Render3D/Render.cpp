#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "File.hpp"
#include "Render.hpp"
#include "TextureLoader.hpp"

namespace Engine {

Render::Render(int width, int height) : m_Width(width), m_Height(height) {
    auto vertexSrc = File::read("./shaders/vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/fragment-shader.glsl");
    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/skybox-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/skybox-fragment-shader.glsl");
    m_SkyboxShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    // vertexSrc = File::read("./shaders/shadow-vertex-shader.glsl");
    // fragmentSrc = File::read("./shaders/shadow-fragment-shader.glsl");
    // m_ShadowShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    // m_DepthMap.reset(TextureLoader::createDepthBuffer(1024 * 2, 1024 * 2));

    // glGenFramebuffers(1, &m_DepthMapFBO);

    // glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    // GL_TEXTURE_2D,
    //                        m_DepthMap->getId(), 0);

    // // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    vertexSrc = File::read("./shaders/cube-shadow-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/cube-shadow-fragment-shader.glsl");
    auto geometrySrc = File::read("./shaders/cube-shadow-geometry-shader.glsl");
    m_CubeShadowShader =
        std::make_unique<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    for (size_t i = 0; i < 4; i++) {
        m_SadowCubeMaps[i] = std::make_unique<CubeMap>(
            1024, 1024, 50.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    glGenFramebuffers(1, &m_DepthCubeMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    //                      m_ShadowCubeMap->texture().getId(), 0);
    // glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /////////////////////////////////////////////////////////////
    /////////////////////// G BUFFER ////////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/g-buffer-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/g-buffer-fragment-shader.glsl");
    m_GBufferShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/deferred-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/deferred-fragment-shader.glsl");
    m_DeferredShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    glGenFramebuffers(1, &m_GBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);

    // буфер позиций
    glGenTextures(1, &m_GPosition);
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_GPosition, 0);

    // буфер нормалей
    glGenTextures(1, &m_GNormal);
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           m_GNormal, 0);

    // буфер для цвета
    glGenTextures(1, &m_GColor);
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           m_GColor, 0);

    // буфер для отражение
    glGenTextures(1, &m_GSpecular);
    glBindTexture(GL_TEXTURE_2D, m_GSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                           m_GSpecular, 0);

    // буфер для глубины
    glGenTextures(1, &m_GDepth);
    glBindTexture(GL_TEXTURE_2D, m_GDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           m_GDepth, 0);

    // укажем OpenGL, какие буферы мы будем использовать при рендеринге
    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    // unsigned int rboDepth;
    // glGenRenderbuffers(1, &rboDepth);
    // glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
    // height); glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    //                           GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /////////////////////////////////////////////////////////////
    ///////////////////////// QUAD //////////////////////////////
    /////////////////////////////////////////////////////////////
    // clang-format off
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // clang-format on
    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void *>(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Render::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Render::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Render::draw(Scene &scene, const ModelManager &models,
                  const Camera &camera) {
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = camera.projectionMatrix();

    /////////////////////////////////////////////////////////////
    //////////////////// DRAW DEPTH MAP /////////////////////////
    /////////////////////////////////////////////////////////////
    // glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    // glCullFace(GL_FRONT);

    // const auto &lightObj = scene.getLights().begin();
    // glm::mat4 lightView = glm::lookAt(lightObj->position, glm::vec3(0.0f),
    //                                   glm::vec3(0.0f, 1.0f, 0.0f));
    // float near_plane = 1.0f, far_plane = 40.0f;
    // glm::mat4 lightProjection =
    //     glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);
    // glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    // m_ShadowShader->setMatrix4("LightSpaceMatrix",
    //                            glm::value_ptr(lightSpaceMatrix));

    if (m_Bake) {
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);

        m_CubeShadowShader->bind();
        unsigned int index = 0;
        for (const auto &lightObj : scene.getLights()) {
            auto &cubeMap = m_SadowCubeMaps[index];

            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 cubeMap->texture().getId(), 0);
            glClear(GL_DEPTH_BUFFER_BIT);

            cubeMap->setPosition(lightObj.position);
            cubeMap->bind(*m_CubeShadowShader);

            drawSceneStaticObjects(*m_CubeShadowShader, scene, models, 1);

            index++;
        }

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_Width, m_Height);
        // glCullFace(GL_BACK);
        m_Bake = true;
    }

    /////////////////////////////////////////////////////////////
    ///////////////////// FILL G BUFFER /////////////////////////
    /////////////////////////////////////////////////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_GBufferShader->bind();
    m_GBufferShader->setMatrix4("View", glm::value_ptr(view));
    m_GBufferShader->setMatrix4("Projection", glm::value_ptr(projection));

    drawSceneObjects(*m_GBufferShader, scene, models, 0);

    /////////////////////////////////////////////////////////////
    ////////////////// SCENE WITH SHADOWS ///////////////////////
    /////////////////////////////////////////////////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_DeferredShader->bind();
    m_DeferredShader->setFloat3("ViewPos", camera.positionVec());
    // m_Shader->setMatrix4("LightSpaceMatrix",
    // glm::value_ptr(lightSpaceMatrix));

    unsigned int index = 0;
    for (const auto &object : scene.getLights()) {
        m_DeferredShader->setFloat3(
            "Lights[" + std::to_string(index) + "].position", object.position);
        m_DeferredShader->setFloat3("Lights[" + std::to_string(index) +
                                        "].ambient",
                                    object.light.ambient);
        m_DeferredShader->setFloat3("Lights[" + std::to_string(index) +
                                        "].diffuse",
                                    object.light.diffuse);
        m_DeferredShader->setFloat3("Lights[" + std::to_string(index) +
                                        "].specular",
                                    object.light.specular);
        m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
                                       "].constant",
                                   object.light.constant);
        m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
                                       "].linear",
                                   object.light.linear);
        m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
                                       "].quadratic",
                                   object.light.quadratic);

        glActiveTexture(GL_TEXTURE0 + index);
        m_SadowCubeMaps[index]->texture().bind();
        m_DeferredShader->setInt("Lights[" + std::to_string(index) +
                                     "].shadowMap",
                                 static_cast<int>(index));
        m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
                                       "].farPlane",
                                   m_SadowCubeMaps[index]->farPlane());

        index++;
    }
    m_DeferredShader->setInt("LightsNumber", static_cast<int>(index));

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glActiveTexture(GL_TEXTURE1 + index);
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glActiveTexture(GL_TEXTURE2 + index);
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    glActiveTexture(GL_TEXTURE3 + index);
    glBindTexture(GL_TEXTURE_2D, m_GSpecular);
    glActiveTexture(GL_TEXTURE4 + index);
    glBindTexture(GL_TEXTURE_2D, m_GDepth);

    m_DeferredShader->setInt("gPosition", 0 + static_cast<int>(index));
    m_DeferredShader->setInt("gNormal", 1 + static_cast<int>(index));
    m_DeferredShader->setInt("gColor", 2 + static_cast<int>(index));
    m_DeferredShader->setInt("gSpecular", 3 + static_cast<int>(index));
    m_DeferredShader->setInt("gDepth", 4 + static_cast<int>(index));

    renderQuad();

    // glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer);
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height,
    //                   GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /////////////////////////////////////////////////////////////
    //////////////////////// SKY BOX ////////////////////////////
    /////////////////////////////////////////////////////////////
    if (scene.getSkybox()) {
        glDepthFunc(GL_LEQUAL);
        m_SkyboxShader->bind();

        auto fixedView = glm::mat4(glm::mat3(view));

        m_SkyboxShader->setMatrix4("View", glm::value_ptr(fixedView));
        m_SkyboxShader->setMatrix4("Projection", glm::value_ptr(projection));

        scene.getSkybox()->draw(*m_SkyboxShader);

        glDepthFunc(GL_LESS);
    }
}

void Render::drawSceneStaticObjects(Shader &shader, Scene &scene,
                                    const ModelManager &models,
                                    unsigned int textureShift) {
    for (auto &pair : scene.getObjects()) {
        for (const auto &mesh : models.GetModel(pair.first)->meshes) {
            auto &instances = pair.second;
            if (instances.resize) {
                mesh.setInstances(instances.GetData());

                instances.resize = false;
                instances.update = false;
            }

            mesh.draw(shader, instances.GetData().size() - instances.staticEnd,
                      textureShift);
        }
    }
}

void Render::drawSceneObjects(Shader &shader, Scene &scene,
                              const ModelManager &models,
                              unsigned int textureShift) {
    for (auto &pair : scene.getObjects()) {
        for (const auto &mesh : models.GetModel(pair.first)->meshes) {
            auto &instances = pair.second;
            if (instances.resize) {
                mesh.setInstances(instances.GetData());
            } else if (instances.update) {
                mesh.updateInstances(instances.from, instances.to,
                                     instances.GetData());
            }

            mesh.draw(shader, instances.GetData().size(), textureShift);

            instances.resize = false;
            instances.update = false;
        }
    }
}

void Render::renderQuad() {
    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

} // namespace Engine
