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

void Framebuffer::addAttachment(const Texture &attachment) {
    switch (attachment.format) {
    case GfxImage::InternalFormat::DEPTH_COMPONENT:
        setDepthAttachment(attachment);
        break;
    default:
        assert(m_AttachmentsIndex < c_MaxFramebufferAttachments && "Too many attachments");

        m_Attachments[m_AttachmentsIndex] = Framebuffer::Attachment(attachment, m_AttachmentsIndex);

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

void Framebuffer::addAttachment(const Renderbuffer &attachment) {
    switch (attachment.format) {
    case GfxImage::InternalFormat::DEPTH_COMPONENT:
        setDepthAttachment(attachment);
        break;
    default:
        assert(m_AttachmentsIndex < c_MaxFramebufferAttachments && "Too many attachments");

        m_Attachments[m_AttachmentsIndex] = Framebuffer::Attachment(attachment, m_AttachmentsIndex);

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

void Framebuffer::setDepthAttachment(const Renderbuffer &attachment) {
    if (attachment.format != Renderbuffer::InternalFormat::DEPTH_COMPONENT) {
        throw std::invalid_argument("Framebuffer::setDepthAttachment: Invalid attachment type.");
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, attachment.id);
    Gfx::checkError();
    m_DepthAttachment = Framebuffer::Attachment(attachment, 0);
}

void Framebuffer::setDepthAttachment(const Texture &attachment) {
    if (attachment.format != Texture::InternalFormat::DEPTH_COMPONENT) {
        throw std::invalid_argument("Framebuffer::setDepthAttachment: Invalid attachment type.");
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment.id, 0);
    Gfx::checkError();
    m_DepthAttachment = Framebuffer::Attachment(attachment, 0);
}

void Framebuffer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Framebuffer::clearDepth() { glClear(GL_DEPTH_BUFFER_BIT); }

void Framebuffer::check() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
}

Framebuffer::Attachment::Attachment() {}

Framebuffer::Attachment::Attachment(const Texture &attachment, unsigned int index)
    : m_Index(index), m_Type(Attachment::Type::Texture) {
    id = attachment.id;
    format = attachment.format;
}

Framebuffer::Attachment::Attachment(const Renderbuffer &attachment, unsigned int index)
    : m_Index(index), m_Type(Attachment::Type::Renderbuffer) {
    id = attachment.id;
    format = attachment.format;
}

void Framebuffer::Attachment::bind() const {}

void Framebuffer::Attachment::unbind() const {}

void Framebuffer::Attachment::resize(unsigned int width, unsigned int height) {}

void Framebuffer::Attachment::read(int x, int y, int width, int height, void *buffer) {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + m_Index);
    glReadPixels(x, y, width, height, GfxImage::getNativeDataFormat(GfxImage::formatToDataFormat(format)),
                 GfxImage::getNativeDataType(GfxImage::formatToDataType(format)), buffer);
    glReadBuffer(GL_NONE);
}

Framebuffer::Attachment &Framebuffer::operator[](unsigned int index) { return m_Attachments[index]; }

} // namespace Engine