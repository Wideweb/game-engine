#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "File.hpp"
#include "Render.hpp"
#include "TextureLoader.hpp"

namespace Engine {

Render::Render(int width, int height) : m_Width(width), m_Height(height) {
    auto vertexSrc = File::read("./shaders/direct-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/direct-fragment-shader.glsl");
    m_DirectShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    /////////////////////////////////////////////////////////////
    //////////////////////// SKY BOX ////////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/skybox-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/skybox-fragment-shader.glsl");
    m_SkyboxShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    /////////////////////////////////////////////////////////////
    //////////////////////// DEPTH //////////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/depth-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/depth-fragment-shader.glsl");
    m_DepthShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_DepthMap.reset(TextureLoader::createDepthBuffer(1024, 1024));

    glGenFramebuffers(1, &m_DepthMapFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap->getId(), 0);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /////////////////////////////////////////////////////////////
    ////////////////////// CUBE SHADOW //////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/cube-shadow-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/cube-shadow-fragment-shader.glsl");
    auto geometrySrc = File::read("./shaders/cube-shadow-geometry-shader.glsl");
    m_CubeShadowShader = std::make_unique<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    for (size_t i = 0; i < 4; i++) {
        m_SadowCubeMaps[i] = std::make_unique<CubeMap>(1024, 1024, 50.0f, glm::vec3(0.0f, 0.0f, 0.0f));
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

    // буфер для цвета
    glGenTextures(1, &m_GColor);
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GColor, 0);

    // буфер позиций
    glGenTextures(1, &m_GPosition);
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GPosition, 0);

    // буфер нормалей
    glGenTextures(1, &m_GNormal);
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GNormal, 0);

    // буфер для отражение
    glGenTextures(1, &m_GSpecular);
    glBindTexture(GL_TEXTURE_2D, m_GSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_GSpecular, 0);

    // буфер для глубины
    glGenTextures(1, &m_GDepth);
    glBindTexture(GL_TEXTURE_2D, m_GDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GDepth, 0);

    // укажем OpenGL, какие буферы мы будем использовать при рендеринге
    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                   GL_COLOR_ATTACHMENT3};
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /////////////////////////////////////////////////////////////
    ///////////////////////// Water /////////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/water-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/water-fragment-shader.glsl");
    m_WaterShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_WaterDudvMap.reset(TextureLoader::loadTextureRGB("./shaders/waterDUDV.png"));
    m_WaterNormalMap.reset(TextureLoader::loadTextureRGB("./shaders/waterNormalMap.png"));

    // clang-format off
    float waterVertices[] = {
        -1.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    };

    // clang-format on
    glGenVertexArrays(1, &m_WaterVAO);
    glGenBuffers(1, &m_WaterVBO);
    glBindVertexArray(m_WaterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_WaterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), &waterVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /////////////////////////////////////////////////////////////
    ////////////////////////// SUN //////////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/sun-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/sun-fragment-shader.glsl");
    m_SunShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_SunTexture.reset(TextureLoader::loadTexture("./assets/lensFlare/sun.png"));

    /////////////////////////////////////////////////////////////
    ////////////////////////// Lens /////////////////////////////
    /////////////////////////////////////////////////////////////
    vertexSrc = File::read("./shaders/screen-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/screen-fragment-shader.glsl");
    m_LensShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

    m_LensFlares[0].reset(TextureLoader::loadTexture("./assets/lensFlare/tex6.png"));
    m_LensFlares[1].reset(TextureLoader::loadTexture("./assets/lensFlare/tex4.png"));
    m_LensFlares[2].reset(TextureLoader::loadTexture("./assets/lensFlare/tex2.png"));
    m_LensFlares[3].reset(TextureLoader::loadTexture("./assets/lensFlare/tex7.png"));
    m_LensFlares[4].reset(TextureLoader::loadTexture("./assets/lensFlare/tex3.png"));
    m_LensFlares[5].reset(TextureLoader::loadTexture("./assets/lensFlare/tex5.png"));
    m_LensFlares[6] = m_LensFlares[3];
    m_LensFlares[7] = m_LensFlares[4];
    m_LensFlares[8] = m_LensFlares[5];
    m_LensFlares[9] = m_LensFlares[1];
    m_LensFlares[10].reset(TextureLoader::loadTexture("./assets/lensFlare/tex8.png"));

    m_LensFlareSize[0] = 0.5f;
    m_LensFlareSize[1] = 0.23f;
    m_LensFlareSize[2] = 0.1f;
    m_LensFlareSize[3] = 0.05f;
    m_LensFlareSize[4] = 0.06f;
    m_LensFlareSize[5] = 0.07f;
    m_LensFlareSize[6] = 0.2f;
    m_LensFlareSize[7] = 0.07f;
    m_LensFlareSize[8] = 0.3f;
    m_LensFlareSize[9] = 0.4f;
    m_LensFlareSize[10] = 0.6f;
}

void Render::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

void Render::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Render::draw(Scene &scene, const ModelManager &models, Camera &camera) {
    glm::vec3 viewPos = camera.positionVec();
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = camera.projectionMatrix();

    /////////////////////////////////////////////////////////////
    //////////////////// DRAW DEPTH MAP /////////////////////////
    /////////////////////////////////////////////////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glViewport(0, 0, 1024, 1024);
    glClear(GL_DEPTH_BUFFER_BIT);

    // const auto &lightObj = scene.getLights().begin();

    float near_plane = 1.0f, far_plane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);

    glm::mat4 lightView =
        glm::lookAt(glm::vec3(-2.0f, 20.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    m_ShadowShader->bind();

    m_ShadowShader->setMatrix4("LightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));

    drawSceneObjects(*m_ShadowShader, scene, models, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_Width, m_Height);

    /////////////////////////////////////////////////////////////
    //////////////////// CUBE DEPTH MAP /////////////////////////
    /////////////////////////////////////////////////////////////
    // if (m_Bake) {
    //     glViewport(0, 0, 1024, 1024);
    //     glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);

    //     m_CubeShadowShader->bind();
    //     unsigned int index = 0;
    //     for (const auto &lightObj : scene.getLights()) {
    //         auto &cubeMap = m_SadowCubeMaps[index];

    //         glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    //                              cubeMap->texture().getId(), 0);
    //         glClear(GL_DEPTH_BUFFER_BIT);

    //         cubeMap->setPosition(lightObj.position);
    //         cubeMap->bind(*m_CubeShadowShader);

    //         drawSceneObjects(*m_CubeShadowShader, scene, models, 1);

    //         index++;
    //     }

    //     // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //     glViewport(0, 0, m_Width, m_Height);
    //     // glCullFace(GL_BACK);
    //     m_Bake = true;
    // }

    /////////////////////////////////////////////////////////////
    ///////////////////// FILL G BUFFER /////////////////////////
    /////////////////////////////////////////////////////////////
    // glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // m_GBufferShader->bind();
    // m_GBufferShader->setMatrix4("View", glm::value_ptr(view));
    // m_GBufferShader->setMatrix4("Projection", glm::value_ptr(projection));
    // m_GBufferShader->setFloat("ClipY", -9999);

    // drawSceneObjects(*m_GBufferShader, scene, models, 0);

    /////////////////////////////////////////////////////////////
    //////////////////// DEFERRED SCENE /////////////////////////
    /////////////////////////////////////////////////////////////
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // m_DeferredShader->bind();
    // m_DeferredShader->setFloat3("ViewPos", viewPos);

    // unsigned int index = 0;
    // for (const auto &object : scene.getLights()) {
    //     m_DeferredShader->setFloat3(
    //         "Lights[" + std::to_string(index) + "].position",
    //         object.position);
    //     m_DeferredShader->setFloat3("Lights[" + std::to_string(index) +
    //                                     "].ambient",
    //                                 object.light.ambient);
    //     m_DeferredShader->setFloat3("Lights[" + std::to_string(index) +
    //                                     "].diffuse",
    //                                 object.light.diffuse);
    //     m_DeferredShader->setFloat3("Lights[" + std::to_string(index) +
    //                                     "].specular",
    //                                 object.light.specular);
    //     m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
    //                                    "].constant",
    //                                object.light.constant);
    //     m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
    //                                    "].linear",
    //                                object.light.linear);
    //     m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
    //                                    "].quadratic",
    //                                object.light.quadratic);
    //     m_DeferredShader->setMatrix4("Lights[" + std::to_string(index) +
    //                                      "].spaceMatrix",
    //                                  glm::value_ptr(lightSpaceMatrix));

    //     glActiveTexture(GL_TEXTURE0 + index);

    //     // m_SadowCubeMaps[index]->texture().bind();
    //     m_DepthMap->bind();
    //     m_DeferredShader->setInt("Lights[" + std::to_string(index) +
    //                                  "].depthMap",
    //                              static_cast<int>(index));
    //     // // m_DeferredShader->setFloat("Lights[" + std::to_string(index) +
    //     // //                                "].farPlane",
    //     //                            m_SadowCubeMaps[index]->farPlane());

    //     index++;
    // }
    // m_DeferredShader->setInt("LightsNumber", static_cast<int>(index));

    // glActiveTexture(GL_TEXTURE0 + index);
    // glBindTexture(GL_TEXTURE_2D, m_GPosition);
    // glActiveTexture(GL_TEXTURE1 + index);
    // glBindTexture(GL_TEXTURE_2D, m_GNormal);
    // glActiveTexture(GL_TEXTURE2 + index);
    // glBindTexture(GL_TEXTURE_2D, m_GColor);
    // glActiveTexture(GL_TEXTURE3 + index);
    // glBindTexture(GL_TEXTURE_2D, m_GSpecular);
    // glActiveTexture(GL_TEXTURE4 + index);
    // glBindTexture(GL_TEXTURE_2D, m_GDepth);

    // m_DeferredShader->setInt("gPosition", 0 + static_cast<int>(index));
    // m_DeferredShader->setInt("gNormal", 1 + static_cast<int>(index));
    // m_DeferredShader->setInt("gColor", 2 + static_cast<int>(index));
    // m_DeferredShader->setInt("gSpecular", 3 + static_cast<int>(index));
    // m_DeferredShader->setInt("gDepth", 4 + static_cast<int>(index));

    // renderQuad();

    /////////////////////////////////////////////////////////////
    /////////////////////////// SCENE ///////////////////////////
    /////////////////////////////////////////////////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_DirectShader->bind();
    m_DirectShader->setFloat3("ViewPos", viewPos);
    m_DirectShader->setMatrix4("View", glm::value_ptr(view));
    m_DirectShader->setMatrix4("Projection", glm::value_ptr(projection));

    unsigned int index = 0;
    for (const auto &object : scene.getLights()) {
        addSpotLight(*m_DirectShader, object.light, object.position, index);
        index++;
    }
    m_DirectShader->setInt("LightsNumber", static_cast<int>(index));

    drawSceneObjects(*m_DirectShader, scene, models, index);

    ///////////////////////////////////////////////////////////
    ////////////////////// SKY BOX ////////////////////////////
    ///////////////////////////////////////////////////////////
    if (scene.getSkybox()) {
        // m_SkyboxRotation += 0.01f;

        glDepthFunc(GL_LEQUAL);
        m_SkyboxShader->bind();

        auto fixedView = glm::mat4(glm::mat3(view));
        auto model = glm::rotate(glm::radians(m_SkyboxRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        m_SkyboxShader->setMatrix4("Model", glm::value_ptr(model));
        m_SkyboxShader->setMatrix4("View", glm::value_ptr(fixedView));
        m_SkyboxShader->setMatrix4("Projection", glm::value_ptr(projection));

        scene.getSkybox()->draw(*m_SkyboxShader);

        glDepthFunc(GL_LESS);
    }

    /////////////////////////////////////////////////////////////
    ///////////////////////// SUN ///////////////////////////////
    /////////////////////////////////////////////////////////////
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 sunPos = glm::vec3(1.5f, 0.5f, 0.0f);
    glm::mat4 sunModel = glm::mat4(1.0f);
    sunModel = glm::translate(sunModel, sunPos);
    sunModel = glm::rotate(sunModel, 1.57f + camera.rotationVec().y, glm::vec3(0.0f, -1.0f, 0.0f));
    sunModel = glm::scale(sunModel, glm::vec3(0.5f));

    m_SunShader->bind();

    m_SunShader->setMatrix4("Model", glm::value_ptr(sunModel));
    m_SunShader->setMatrix4("View", glm::value_ptr(glm::mat4(glm::mat3(view))));
    m_SunShader->setMatrix4("Projection", glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    m_SunTexture->bind();
    m_SunShader->setInt("colorMap", 0);

    renderQuad();

    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);

    /////////////////////////////////////////////////////////////
    //////////////////////// Water //////////////////////////////
    /////////////////////////////////////////////////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();

    m_GBufferShader->bind();
    m_GBufferShader->setMatrix4("View", glm::value_ptr(camera.viewMatrix()));
    m_GBufferShader->setMatrix4("Projection", glm::value_ptr(projection));
    m_GBufferShader->setFloat("ClipY", 0);

    drawSceneObjects(*m_GBufferShader, scene, models, 0);

    if (scene.getSkybox()) {
        glDepthFunc(GL_LEQUAL);
        m_SkyboxShader->bind();

        auto fixedView = glm::mat4(glm::mat3(camera.viewMatrix()));
        auto model = glm::rotate(glm::radians(m_SkyboxRotation), glm::vec3(0.0f, 1.0f, 0.0f));

        m_SkyboxShader->setMatrix4("Model", glm::value_ptr(model));
        m_SkyboxShader->setMatrix4("View", glm::value_ptr(fixedView));
        m_SkyboxShader->setMatrix4("Projection", glm::value_ptr(projection));

        scene.getSkybox()->draw(*m_SkyboxShader);

        glDepthFunc(GL_LESS);
    }

    camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
    camera.inversePitch();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_WaterShader->bind();

    glm::mat4 model(1);
    model = glm::translate(model, glm::vec3(glm::vec3(10.0, 0.0, 10.0)));
    model = glm::scale(model, glm::vec3(10.0));

    m_WaterShader->setFloat3("ViewPos", viewPos);
    m_WaterShader->setMatrix4("View", glm::value_ptr(view));
    m_WaterShader->setMatrix4("Projection", glm::value_ptr(projection));
    m_WaterShader->setMatrix4("Model", glm::value_ptr(model));

    m_WaterShader->setFloat("moveFactor", m_WaterMoveFactor);
    m_WaterMoveFactor += 0.001f;

    m_WaterShader->setFloat3("lightDir", scene.getLights().begin()->position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GColor);
    m_WaterShader->setInt("colorMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GDepth);
    m_WaterShader->setInt("depthMap", 1);

    glActiveTexture(GL_TEXTURE2);
    m_WaterDudvMap->bind();
    m_WaterShader->setInt("dudvMap", 2);

    glActiveTexture(GL_TEXTURE3);
    m_WaterNormalMap->bind();
    m_WaterShader->setInt("normalMap", 3);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(m_WaterVBO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

    ///////////////////////////////////////////////////////////
    ////////////////////// Flare //////////////////////////////
    ///////////////////////////////////////////////////////////
    // glm::vec4 sunScreenPos =
    //     projection * glm::mat4(glm::mat3(view)) * glm::vec4(sunPos, 1.0f);
    // float x = (sunScreenPos.x / sunScreenPos.w + 1.0f) / 2.0f;
    // float y = (sunScreenPos.y / sunScreenPos.w + 1.0f) / 2.0f;

    // glm::vec2 sunToCenter = glm::vec2(0.5f) - glm::vec2(x, y);
    // float brightness = (1.0f - glm::length(sunToCenter) / 0.6f) * 0.15f;

    // if (brightness > 0 && sunScreenPos.z > 0) {
    //     glDisable(GL_DEPTH_TEST);
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //     m_LensShader->bind();

    //     m_LensShader->setFloat("alpha", brightness);

    //     for (unsigned int i = 0; i < m_LensFlares.size(); i++) {
    //         glm::vec2 flarePos = glm::vec2(sunScreenPos / sunScreenPos.w) +
    //                              sunToCenter * (0.4f * i);
    //         glm::mat4 flareModel = glm::mat4(1.0f);
    //         flareModel = glm::translate(flareModel, glm::vec3(flarePos,
    //         0.0f)); flareModel = glm::scale(
    //             flareModel,
    //             glm::vec3(m_LensFlareSize[i],
    //                       m_LensFlareSize[i] * m_Width / m_Height, 0.0f));

    //         m_LensShader->setMatrix4("Model", glm::value_ptr(flareModel));

    //         glActiveTexture(GL_TEXTURE0);
    //         m_LensFlares[i]->bind();
    //         m_LensShader->setInt("colorMap", 0);

    //         renderQuad();
    //     }

    //     glEnable(GL_DEPTH_TEST);
    //     glDisable(GL_BLEND);
    // }

    // DEBUG

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_LensShader->bind();

    glm::mat4 screenModel = glm::mat4(1.0f);
    screenModel = glm::scale(screenModel, glm::vec3(0.2f));

    m_LensShader->setFloat("alpha", 1.0f);
    m_LensShader->setMatrix4("Model", glm::value_ptr(screenModel));

    glActiveTexture(GL_TEXTURE0);
    m_DepthMap->bind();
    m_LensShader->setInt("colorMap", 0);

    renderQuad();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

// void Render::drawSceneStaticObjects(Shader &shader, Scene &scene,
//                                     const ModelManager &models,
//                                     unsigned int textureShift) {
//     for (auto &pair : scene.getObjects()) {
//         for (const auto &mesh : models.GetModel(pair.first)->meshes) {
//             auto &instances = pair.second;
//             if (instances.resize) {
//                 mesh.setInstances(instances.GetData());

//                 instances.resize = false;
//                 instances.update = false;
//             }

//             mesh.draw(shader, instances.GetData().size() -
//             instances.staticEnd,
//                       textureShift);
//         }
//     }
// }

void Render::drawSceneObjects(Shader &shader, Scene &scene, const ModelManager &models, unsigned int textureShift) {
    for (auto &pair : scene.getObjects()) {
        const auto &model = models.GetModel(pair.first);
        auto &instances = pair.second;
        model->draw(shader, instances, textureShift);
    }
}

void Render::renderQuad() {
    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void setDirectedLight(Shader &shader, const Light &light, const glm::vec3 &position) {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_Width, m_Height);

    shader.setFloat3("u_directedLight.position", position);
    shader.setFloat3("u_directedLight.ambient", light.ambient);
    shader.setFloat3("u_directedLight.diffuse", light.diffuse);
    shader.setFloat3("u_directedLight.specular", light.specular);
    shader.setFloat("u_directedLight.constant", light.constant);
    shader.setFloat("u_directedLight.linear", light.linear);
    shader.setFloat("u_directedLight.quadratic", light.quadratic);
    shader.setMatrix4("u_directedLight.spaceMatrix", glm::value_ptr(lightSpaceMatrix));

    glActiveTexture(GL_TEXTURE0);
    m_DepthMap->bind();
    shader.setInt("u_directedLight.shadowMap", static_cast<int>(index));
}

void addSpotLight(Shader &shader, const Light &light, const glm::vec3 &position, unsigned int index) {
    shader.setFloat3("Lights[" + std::to_string(index) + "].position", position);
    shader.setFloat3("Lights[" + std::to_string(index) + "].ambient", light.ambient);
    shader.setFloat3("Lights[" + std::to_string(index) + "].diffuse", light.diffuse);
    shader.setFloat3("Lights[" + std::to_string(index) + "].specular", light.specular);
    shader.setFloat("Lights[" + std::to_string(index) + "].constant", light.constant);
    shader.setFloat("Lights[" + std::to_string(index) + "].linear", light.linear);
    shader.setFloat("Lights[" + std::to_string(index) + "].quadratic", light.quadratic);

    glActiveTexture(GL_TEXTURE0 + index);
    m_DepthMap->bind();
    shader.setInt("shadowMap", static_cast<int>(index));

    shader.setMatrix4("LightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
}

} // namespace Engine
