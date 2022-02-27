#pragma once

#include "Renderbuffer.hpp"
#include "Texture.hpp"

#include <array>

namespace Engine {

constexpr unsigned int c_MaxFramebufferAttachments = 5;

class Framebuffer : public GfxObject {
  private:
    class Attachment : public GfxImage {
      private:
        enum class Type { None, Texture, Renderbuffer };

        unsigned int m_Index = 0;
        Type m_Type = Type::None;

      public:
        Attachment();
        Attachment(const Texture &attachment, unsigned int index);
        Attachment(const Renderbuffer &attachment, unsigned int index);

        void bind() const override;
        void unbind() const override;

        void resize(unsigned int width, unsigned int height) override;

        void read(int x, int y, int width, int height, void *buffer);
    };

    unsigned int m_AttachmentsIndex = 0;
    std::array<Attachment, c_MaxFramebufferAttachments> m_Attachments;
    Attachment m_DepthAttachment;

  public:
    Attachment &operator[](unsigned int index);

    void bind() const override;
    void unbind() const override;
    void addAttachment(const Texture &attachment);
    void addAttachment(const Renderbuffer &attachment);
    void setDepthAttachment(const Texture &attachment);
    void setDepthAttachment(const Renderbuffer &attachment);

    void clear();
    void clearDepth();
    void check();

    static Framebuffer createDefault();
    static Framebuffer create();
};

} // namespace Engine