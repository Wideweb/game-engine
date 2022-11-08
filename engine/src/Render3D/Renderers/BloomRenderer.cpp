#include "BloomRenderer.hpp"

#include "File.hpp"

#include "glad/glad.h"

namespace Engine {

BloomRenderer::BloomRenderer(Viewport &viewport, QuadRenderer &quadRenderer) : m_Viewport(viewport), m_QuadRenderer(quadRenderer) {
    auto vertexSrc = File::readGLSL("./shaders/pass/common/clipping-space-textured.vertex.glsl");
    auto fragmentSrc = File::readGLSL("./shaders/pass/blur-gaussian.fragment.glsl");
    m_BlurShader = Shader(vertexSrc, fragmentSrc);

    fragmentSrc = File::readGLSL("./shaders/pass/common/textured.fragment.glsl");
    m_BlitShader = Shader(vertexSrc, fragmentSrc);

    fragmentSrc = File::readGLSL("./shaders/pass/bloom.fragment.glsl");
    m_BloomShader = Shader(vertexSrc, fragmentSrc);

    int width = m_Viewport.width;
    int height = m_Viewport.height;
    for (int i = 0; i < MIP_COUNT; i++)
	{
		width /= 2;
		height /= 2;
		for (int j = 0; j < 2; j++)
		{
            m_MipColorBuffer[i][j] = Texture::createRGBA16FBuffer(width, height);
            m_MipColorBuffer[i][j].bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            m_MipColorBuffer[i][j].unbind();

            m_MipFramebuffer[i][j] = Framebuffer::create();
            m_MipFramebuffer[i][j].bind();
            m_MipFramebuffer[i][j].addAttachment(m_MipColorBuffer[i][j]);
            m_MipFramebuffer[i][j].unbind();
            
		}
	}
}

BloomRenderer::~BloomRenderer() { m_BlurShader.free(); }

void BloomRenderer::draw(RenderSettings &settings, Texture inputColorBuffer, Texture exposureColorBuffer, Framebuffer outputFramebuffer) {
    glDisable(GL_DEPTH_TEST);

    unsigned int lastViewportWidth = m_Viewport.width;
    unsigned int lastViewportHeight = m_Viewport.height;

    int width = m_Viewport.width / 2;
    int height = m_Viewport.height / 2;
    m_Viewport.resize(width, height);

    m_BloomShader.bind();
    m_BloomShader.setTexture("u_colorBuffer", inputColorBuffer);
    m_BloomShader.setTexture("u_exposure", exposureColorBuffer);
    m_BloomShader.setFloat("u_threshold", settings.threshold);
    m_BloomShader.setFloat("u_scale",  0.02f);
    m_MipFramebuffer[0][1].bind();
    m_QuadRenderer.draw();

    m_BlurShader.bind();
    for (int i = 1; i < MIP_COUNT; i++)
	{
        width /= 2;
		height /= 2;
        m_Viewport.resize(width, height);
        {
            m_BlurShader.setInt("u_horizontal", true);
            m_BlurShader.setTexture("u_colorBuffer", m_MipColorBuffer[i - 1][1]);
            m_MipFramebuffer[i][0].bind();
            m_QuadRenderer.draw();
        }

        {
            m_BlurShader.setInt("u_horizontal", false);
            m_BlurShader.setTexture("u_colorBuffer", m_MipColorBuffer[i][0]);
            m_MipFramebuffer[i][1].bind();
            m_QuadRenderer.draw();
        }
	}
    
    m_Viewport.resize(lastViewportWidth, lastViewportHeight);

    m_BlitShader.bind();
    outputFramebuffer.bind();

    glDisable(GL_BLEND);

    m_BlitShader.setTexture("u_colorBuffer", m_MipColorBuffer[0][1]);
    m_QuadRenderer.draw();

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int i = 1; i < MIP_COUNT; i++)
    {
        m_BlitShader.setTexture("u_colorBuffer", m_MipColorBuffer[i][1]);
        m_QuadRenderer.draw();
    }
    outputFramebuffer.unbind();
    m_BlitShader.unbind();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void BloomRenderer::resize() {
    int width = m_Viewport.width;
    int height = m_Viewport.height;
    for (int i = 0; i < MIP_COUNT; i++)
    {
        width /= 2;
        height /= 2;
        for (int j = 0; j < 2; j++)
        {
            m_MipColorBuffer[i][j].bind();
            m_MipColorBuffer[i][j].resize(width, height);
            m_MipColorBuffer[i][j].unbind();
            
        }
    }
}

} // namespace Engine