#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>

#include "Camera.hpp"
#include "CubeMap.hpp"
#include "Light.hpp"
#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "File.hpp"

namespace Engine {

class Render {
  private:
    std::unique_ptr<Shader> m_DirectShader;

    std::unique_ptr<Shader> m_SkyboxShader;
    float m_SkyboxRotation = 0.0f;

    std::unique_ptr<Shader> m_DepthShader;
    unsigned int m_DepthMapFBO;
    std::unique_ptr<Texture> m_DepthMap;

    std::unique_ptr<Shader> m_CubeShadowShader;
    unsigned int m_DepthCubeMapFBO;
    std::array<std::unique_ptr<CubeMap>, 4> m_SadowCubeMaps;

    std::unique_ptr<Shader> m_GBufferShader;
    std::unique_ptr<Shader> m_DeferredShader;
    unsigned int m_GBuffer;
    unsigned int m_GPosition, m_GNormal, m_GColor, m_GSpecular, m_GDepth;

    unsigned int m_QuadVAO, m_QuadVBO;

    std::unique_ptr<Shader> m_WaterShader;
    unsigned int m_WaterVAO, m_WaterVBO;
    std::shared_ptr<Texture> m_WaterDudvMap;
    std::shared_ptr<Texture> m_WaterNormalMap;
    float m_WaterMoveFactor = 0.0f;

    std::unique_ptr<Texture> m_SunTexture;
    std::unique_ptr<Shader> m_SunShader;

    std::unique_ptr<Shader> m_LensShader;
    std::array<std::shared_ptr<Texture>, 11> m_LensFlares;
    std::array<float, 11> m_LensFlareSize;

    int m_Width;
    int m_Height;

  public:
    Render(int width, int height);

    void draw(Scene &scene, const ModelManager &models, Camera &camera);
    void setClearColor(float r, float g, float b, float a);
    void clear();

  private:
    // void drawSceneStaticObjects(Shader &shader, Scene &scene,
    //                             const ModelManager &models,
    //                             unsigned int textureShift);
    void drawSceneObjects(Shader &shader, Scene &scene, const ModelManager &models, unsigned int textureShift);
    void renderQuad();

    void setDirectedLight(Shader &shader, const Light &light, const glm::vec3 &position);
    void addSpotLight(Shader &shader, const Light &light, const glm::vec3 &position, unsigned int index);
};

class QuadRender {
  private:
    unsigned int m_QuadVAO, m_QuadVBO;

  public:
    QuadRender() {
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
    }

    void draw() {
        glBindVertexArray(m_QuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
};

class ModelRender {
  public:
    void draw(Shader &shader, Scene &scene, const ModelManager &models, unsigned int textureShift) {
        for (auto &pair : scene.getObjects()) {
            const auto &model = models.GetModel(pair.first);
            auto &instances = pair.second;
            model->draw(shader, instances, textureShift);
        }
    }
};

class SkyboxRender {
  private:
    std::unique_ptr<Shader> m_SkyboxShader;
    float m_SkyboxRotation = 0.0f;

  public:
    SkyboxRender() {
        std::string vertexSrc = File::read("./shaders/skybox-vertex-shader.glsl");
        std::string fragmentSrc = File::read("./shaders/skybox-fragment-shader.glsl");
        m_SkyboxShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
    }

    void draw(Camera &camera, Scene &scene) {
        if (scene.getSkybox()) {
            // m_SkyboxRotation += 0.01f;

            glDepthFunc(GL_LEQUAL);
            m_SkyboxShader->bind();

            auto fixedView = glm::mat4(glm::mat3(camera.viewMatrix()));
            auto model = glm::rotate(glm::radians(m_SkyboxRotation), glm::vec3(0.0f, 1.0f, 0.0f));

            m_SkyboxShader->setMatrix4("Model", model);
            m_SkyboxShader->setMatrix4("View", fixedView);
            m_SkyboxShader->setMatrix4("Projection", camera.projectionMatrix());

            scene.getSkybox()->draw(*m_SkyboxShader);

            glDepthFunc(GL_LESS);
        }
    }
};

class DirectLightRender {
  private:
    std::unique_ptr<Shader> m_DepthShader;
    unsigned int m_DepthMapFBO;
    std::unique_ptr<Texture> m_DepthMap;

    ModelRender &m_ModelRender;

  public:
    DirectLightRender(ModelRender &modelRender) : m_ModelRender(modelRender) {
        std::string vertexSrc = File::read("./shaders/depth-vertex-shader.glsl");
        std::string fragmentSrc = File::read("./shaders/depth-fragment-shader.glsl");
        m_DepthShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

        m_DepthMap.reset(TextureLoader::createDepthBuffer(1024, 1024));

        glGenFramebuffers(1, &m_DepthMapFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap->getId(), 0);

        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void apply(const Light &light, const glm::vec3 &position, Shader &shader, Scene &scene,
               const ModelManager &models) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
        glViewport(0, 0, 1024, 1024);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, light.nearPlane, light.farPlane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), light.direction, glm::vec3(0.0f, -1.0f, 0.0f));

        m_DepthShader->bind();

        m_DepthShader->setMatrix4("u_view", lightView);
        m_DepthShader->setMatrix4("u_projection", lightProjection);

        m_ModelRender.draw(*m_DepthShader, scene, models, 0);

        shader.bind();
        shader.setFloat3("u_directedLight.position", position);
        shader.setFloat3("u_directedLight.ambient", light.ambient);
        shader.setFloat3("u_directedLight.diffuse", light.diffuse);
        shader.setFloat3("u_directedLight.specular", light.specular);
        shader.setFloat("u_directedLight.constant", light.constant);
        shader.setFloat("u_directedLight.linear", light.linear);
        shader.setFloat("u_directedLight.quadratic", light.quadratic);
        shader.setMatrix4("u_directedLight.spaceMatrix", lightProjection * lightView);

        glActiveTexture(GL_TEXTURE0);
        m_DepthMap->bind();
        shader.setInt("u_directedLight.shadowMap", 0);
    }
};

class SpotLightRender {
  private:
    std::unique_ptr<Shader> m_CubeShadowShader;
    unsigned int m_DepthCubeMapFBO;
    std::array<std::unique_ptr<CubeMap>, 4> m_SadowCubeMaps;

    ModelRender &m_ModelRender;
    unsigned int m_ActiveLights = 0;

  public:
    SpotLightRender(ModelRender &modelRender) : m_ModelRender(modelRender) {
        std::string vertexSrc = File::read("./shaders/cube-shadow-vertex-shader.glsl");
        std::string fragmentSrc = File::read("./shaders/cube-shadow-fragment-shader.glsl");
        auto geometrySrc = File::read("./shaders/cube-shadow-geometry-shader.glsl");
        m_CubeShadowShader = std::make_unique<Shader>(vertexSrc, fragmentSrc, geometrySrc);

        for (size_t i = 0; i < 4; i++) {
            m_SadowCubeMaps[i] = std::make_unique<CubeMap>(1024, 1024, 50.0f, glm::vec3(0.0f, 0.0f, 0.0f));
        }

        glGenFramebuffers(1, &m_DepthCubeMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void apply(const Light &light, const glm::vec3 &position, Shader &shader, Scene &scene,
               const ModelManager &models) {
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeMapFBO);

        auto &cubeMap = m_SadowCubeMaps[m_ActiveLights];

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap->texture().getId(), 0);
        glClear(GL_DEPTH_BUFFER_BIT);

        m_CubeShadowShader->bind();
        cubeMap->setPosition(position);
        cubeMap->bind(*m_CubeShadowShader);

        m_ModelRender.draw(*m_CubeShadowShader, scene, models, 1);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_Width, m_Height);
        // glCullFace(GL_BACK);

        shader.bind();
        shader.setFloat3("Lights[" + std::to_string(m_ActiveLights) + "].position", position);
        shader.setFloat3("Lights[" + std::to_string(m_ActiveLights) + "].ambient", light.ambient);
        shader.setFloat3("Lights[" + std::to_string(m_ActiveLights) + "].diffuse", light.diffuse);
        shader.setFloat3("Lights[" + std::to_string(m_ActiveLights) + "].specular", light.specular);
        shader.setFloat("Lights[" + std::to_string(m_ActiveLights) + "].constant", light.constant);
        shader.setFloat("Lights[" + std::to_string(m_ActiveLights) + "].linear", light.linear);
        shader.setFloat("Lights[" + std::to_string(m_ActiveLights) + "].quadratic", light.quadratic);

        glActiveTexture(GL_TEXTURE0 + m_ActiveLights);
        m_SadowCubeMaps[m_ActiveLights]->texture().bind();
        shader.setInt("Lights[" + std::to_string(m_ActiveLights) + "].depthMap", static_cast<int>(m_ActiveLights));
        shader.setFloat("Lights[" + std::to_string(m_ActiveLights) + "].farPlane",
                        m_SadowCubeMaps[m_ActiveLights]->farPlane());

        m_ActiveLights++;
        shader.setInt("LightsNumber", m_ActiveLights);
    }
};

class WaterRender {
  private:
    std::unique_ptr<Shader> m_WaterShader;
    unsigned int m_WaterVAO, m_WaterVBO;
    std::shared_ptr<Texture> m_WaterDudvMap;
    std::shared_ptr<Texture> m_WaterNormalMap;
    float m_WaterMoveFactor = 0.0f;

    std::unique_ptr<Shader> m_GBufferShader;
    unsigned int m_GBuffer;
    unsigned int m_GPosition, m_GNormal, m_GColor, m_GSpecular, m_GDepth;

    ModelRender &m_ModelRender;
    SkyboxRender &m_SkyboxRender;

  public:
    WaterRender(ModelRender &modelRender, SkyboxRender &skyboxRender)
        : m_ModelRender(modelRender), m_SkyboxRender(skyboxRender) {
        std::string vertexSrc = File::read("./shaders/water-vertex-shader.glsl");
        std::string fragmentSrc = File::read("./shaders/water-fragment-shader.glsl");
        m_WaterShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

        m_WaterDudvMap.reset(TextureLoader::loadTexture("./shaders/waterDUDV.png"));
        m_WaterNormalMap.reset(TextureLoader::loadTexture("./shaders/waterNormalMap.png"));

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
    }

    void draw(Camera &camera, Scene &scene, const ModelManager &models, Shader &shader) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
        camera.inversePitch();

        m_GBufferShader->bind();
        m_GBufferShader->setMatrix4("View", camera.viewMatrix());
        m_GBufferShader->setMatrix4("Projection", camera.projectionMatrix());
        m_GBufferShader->setFloat("ClipY", 0);

        m_ModelRender.draw(*m_GBufferShader, scene, models, 1);
        m_SkyboxRender.draw(camera, scene);

        camera.setPosition(camera.positionVec() * glm::vec3(1.0, -1.0, 1.0));
        camera.inversePitch();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_WaterShader->bind();

        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(glm::vec3(10.0, 0.0, 10.0)));
        model = glm::scale(model, glm::vec3(10.0));

        m_WaterShader->setFloat3("ViewPos", camera.positionVec());
        m_WaterShader->setMatrix4("View", camera.viewMatrix());
        m_WaterShader->setMatrix4("Projection", camera.projectionMatrix());
        m_WaterShader->setMatrix4("Model", model);

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
    }
};

class FlareRender {
  private:
    std::unique_ptr<Shader> m_LensShader;
    std::array<std::shared_ptr<Texture>, 11> m_LensFlares;
    std::array<float, 11> m_LensFlareSize;

    QuadRender &m_QuadRender;

  public:
    FlareRender(QuadRender quadRender) : m_QuadRender(quadRender) {
        std::string vertexSrc = File::read("./shaders/screen-vertex-shader.glsl");
        std::string fragmentSrc = File::read("./shaders/screen-fragment-shader.glsl");
        std::string m_LensShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

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

    void draw(Camera &camera, Scene &scene, const glm::vec4 lightPosition) {
        glm::vec4 sunScreenPos = camera.projectionMatrix() * glm::mat4(glm::mat3(camera.viewMatrix()) * glm::vec4(lightPosition, 1.0f);
        float x = (sunScreenPos.x / sunScreenPos.w + 1.0f) / 2.0f;
        float y = (sunScreenPos.y / sunScreenPos.w + 1.0f) / 2.0f;

        glm::vec2 sunToCenter = glm::vec2(0.5f) - glm::vec2(x, y);
        float brightness = (1.0f - glm::length(sunToCenter) / 0.6f) * 0.15f;

        if (brightness > 0 && sunScreenPos.z > 0) {
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            m_LensShader->bind();

            m_LensShader->setFloat("alpha", brightness);

            for (unsigned int i = 0; i < m_LensFlares.size(); i++) {
                glm::vec2 flarePos = glm::vec2(sunScreenPos / sunScreenPos.w) + sunToCenter * (0.4f * i);
                glm::mat4 flareModel = glm::mat4(1.0f);
                flareModel = glm::translate(flareModel, glm::vec3(flarePos, 0.0f));
                flareModel = glm::scale(flareModel,
                                        glm::vec3(m_LensFlareSize[i], m_LensFlareSize[i] * m_Width / m_Height, 0.0f));

                m_LensShader->setMatrix4("Model", flareModel);

                glActiveTexture(GL_TEXTURE0);
                m_LensFlares[i]->bind();
                m_LensShader->setInt("colorMap", 0);

                m_QuadRender.draw();
            }

            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }
    }
};

class MasterRender {
  private:
    DirectLightRender m_DirectLightRender;
    SpotLightRender m_SpotLightRender;
    ModelRender m_ModelRender;

  public:
    void draw(Scene &scene, const ModelManager &models, Camera &camera) {}
};

} // namespace Engine