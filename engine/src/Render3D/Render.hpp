#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>

#include "Camera.hpp"
#include "CubeMap.hpp"
#include "ModelManager.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Engine {

class Render {
  private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Shader> m_SkyboxShader;

    // std::unique_ptr<Shader> m_ShadowShader;
    // unsigned int m_DepthMapFBO;
    // std::unique_ptr<Texture> m_DepthMap;

    std::unique_ptr<Shader> m_CubeShadowShader;
    unsigned int m_DepthCubeMapFBO;
    std::array<std::unique_ptr<CubeMap>, 4> m_SadowCubeMaps;
    bool m_Bake = true;

    std::unique_ptr<Shader> m_GBufferShader;
    std::unique_ptr<Shader> m_DeferredShader;
    unsigned int m_GBuffer;
    unsigned int m_GPosition, m_GNormal, m_GColor, m_GSpecular, m_GDepth;

    unsigned int m_QuadVAO, m_QuadVBO;

    int m_Width;
    int m_Height;

  public:
    Render(int width, int height);

    void draw(Scene &scene, const ModelManager &models, const Camera &camera);
    void setClearColor(float r, float g, float b, float a);
    void clear();

  private:
    void drawSceneStaticObjects(Shader &shader, Scene &scene,
                                const ModelManager &models,
                                unsigned int textureShift);
    void drawSceneObjects(Shader &shader, Scene &scene,
                          const ModelManager &models,
                          unsigned int textureShift);
    void renderQuad();
};

} // namespace Engine
