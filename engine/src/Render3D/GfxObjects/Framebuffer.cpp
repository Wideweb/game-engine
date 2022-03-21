#include "Framebuffer.hpp"

#include "GfxUtils.hpp"

#include "glad/glad.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace Engine {

Framebuffer Framebuffer::createDefault() {
    Framebuffer framebuffer;
    framebuffer.id = 0;
    return framebuffer;
}

Framebuffer Framebuffer::create() {
    Framebuffer framebuffer;

    glGenFramebuffers(1, &framebuffer.id);

    return framebuffer;
}

void Framebuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id); }

void Framebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::resize(unsigned int width, unsigned int height) {
    // for (unsigned int index = 0; index < m_AttachmentsIndex; index++) {
    //     m_Attachments[index].resize();
    // }
    m_DepthAttachment.bind();
    m_DepthAttachment.resize(width, height);
    m_DepthAttachment.unbind();
}

void Framebuffer::free() {
    if (!empty()) {
        glDeleteFramebuffers(1, &id);
        setEmpty();

        for (unsigned int index = 0; index < m_AttachmentsIndex; index++) {
            m_Attachments[index].free();
        }
        m_DepthAttachment.free();
        m_AttachmentsIndex = 0;
    }
}

void Framebuffer::addAttachment(const Texture &attachment, bool own) {
    switch (attachment.format) {
    case GfxImage::InternalFormat::DEPTH_COMPONENT:
        setDepthAttachment(attachment);
        break;
    default:
        assert(m_AttachmentsIndex < c_MaxFramebufferAttachments && "Too many attachments");

        m_Attachments[m_AttachmentsIndex] = Framebuffer::Attachment(attachment, m_AttachmentsIndex, own);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_AttachmentsIndex, GL_TEXTURE_2D, attachment.id,
                               0);
        Gfx::checkError();

        ++m_AttachmentsIndex;
        std::vector<unsigned int> drawAttachments;
        for (unsigned int index = 0; index < m_AttachmentsIndex; index++) {
            drawAttachments.push_back(GL_COLOR_ATTACHMENT0 + index);
        }
        glDrawBuffers(drawAttachments.size(), drawAttachments.data());
        Gfx::checkError();
        break;
    }
}

void Framebuffer::addAttachment(const Renderbuffer &attachment, bool own) {
    switch (attachment.format) {
    case GfxImage::InternalFormat::DEPTH_COMPONENT:
        setDepthAttachment(attachment);
        break;
    default:
        assert(m_AttachmentsIndex < c_MaxFramebufferAttachments && "Too many attachments");

        m_Attachments[m_AttachmentsIndex] = Framebuffer::Attachment(attachment, m_AttachmentsIndex, own);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_AttachmentsIndex, GL_RENDERBUFFER,
                                  attachment.id);
        Gfx::checkError();
        ++m_AttachmentsIndex;
        std::vector<unsigned int> drawAttachments;
        for (unsigned int index = 0; index < m_AttachmentsIndex; index++) {
            drawAttachments.push_back(GL_COLOR_ATTACHMENT0 + index);
        }
        glDrawBuffers(drawAttachments.size(), drawAttachments.data());
        Gfx::checkError();
        break;
    }
}

void Framebuffer::setDepthAttachment(const Renderbuffer &attachment, bool own) {
    if (attachment.format != Renderbuffer::InternalFormat::DEPTH_COMPONENT) {
        throw std::invalid_argument("Framebuffer::setDepthAttachment: Invalid attachment type.");
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, attachment.id);
    Gfx::checkError();
    m_DepthAttachment = Framebuffer::Attachment(attachment, 0, own);
}

void Framebuffer::setDepthAttachment(const Texture &attachment, bool own) {
    if (attachment.format != Texture::InternalFormat::DEPTH_COMPONENT) {
        throw std::invalid_argument("Framebuffer::setDepthAttachment: Invalid attachment type.");
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment.id, 0);
    Gfx::checkError();
    m_DepthAttachment = Framebuffer::Attachment(attachment, 0, own);
}

void Framebuffer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Framebuffer::clearDepth() { glClear(GL_DEPTH_BUFFER_BIT); }

void Framebuffer::check() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
}

Framebuffer::Attachment::Attachment() {}

Framebuffer::Attachment::Attachment(const Texture &attachment, unsigned int index, bool owned)
    : m_Index(index), m_Type(Attachment::Type::Texture), m_Owned(owned) {
    id = attachment.id;
    format = attachment.format;
}

Framebuffer::Attachment::Attachment(const Renderbuffer &attachment, unsigned int index, bool owned)
    : m_Index(index), m_Type(Attachment::Type::Renderbuffer), m_Owned(owned) {
    id = attachment.id;
    format = attachment.format;
}

void Framebuffer::Attachment::bind() const {}

void Framebuffer::Attachment::unbind() const {}

void Framebuffer::Attachment::free() {
    if (!m_Owned || empty()) {
        return;
    }

    if (m_Type == Attachment::Type::Texture) {
        glDeleteTextures(1, &id);
        setEmpty();
    }

    if (m_Type == Attachment::Type::Renderbuffer) {
        glDeleteRenderbuffers(1, &id);
        setEmpty();
    }
}

void Framebuffer::Attachment::resize(unsigned int width, unsigned int height) {
    if (!m_Owned || empty()) {
        return;
    }

    // if (m_Type == Attachment::Type::Texture) {
    //     glTexImage2D(getGLTextureType(type), 0, GfxImage::getNativeFormat(format), width, height, 0,
    //                  GfxImage::getNativeDataFormat(dataFormat), GfxImage::getNativeDataType(dataType), NULL);
    // }

    if (m_Type == Attachment::Type::Renderbuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, id);
        glRenderbufferStorage(GL_RENDERBUFFER, GfxImage::getNativeFormat(format), width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

void Framebuffer::Attachment::read(int x, int y, int width, int height, void *buffer) {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + m_Index);
    glReadPixels(x, y, width, height, GfxImage::getNativeDataFormat(GfxImage::formatToDataFormat(format)),
                 GfxImage::getNativeDataType(GfxImage::formatToDataType(format)), buffer);
    glReadBuffer(GL_NONE);
}

Framebuffer::Attachment &Framebuffer::operator[](unsigned int index) { return m_Attachments[index]; }

} // namespace Engine