#pragma once

#include "Renderbuffer.hpp"
#include "Texture.hpp"

#include <array>
#include <glm/vec4.hpp>

namespace Engine {

constexpr unsigned int c_MaxFramebufferAttachments = 5;

class Framebuffer : public GfxObject {
  private:
    class Attachment : public GfxImage {
      private:
        enum class Type { None, Texture, Renderbuffer };

        unsigned int m_Index = 0;
        Type m_Type = Type::None;
        bool m_Owned = false;

      public:
        Attachment();
        Attachment(const Texture &attachment, unsigned int index, bool owned = false);
        Attachment(const Renderbuffer &attachment, unsigned int index, bool owned = false);

        void bind() const override;
        void unbind() const override;
        void free() override;

        void resize(unsigned int width, unsigned int height) override;

        void read(int x, int y, int width, int height, void *buffer);
        void clear(glm::vec4 color);
    };

    unsigned int m_AttachmentsIndex = 0;
    std::array<Attachment, c_MaxFramebufferAttachments> m_Attachments;
    Attachment m_DepthAttachment;

    void freeAttachment(Attachment &attachment);

  public:
    Attachment &operator[](unsigned int index);

    bool operator==(Framebuffer &other) { return other.id == id; };

    void bind() const override;
    void unbind() const override;
    void resize(unsigned int width, unsigned int height);
    void free() override;

    void addAttachment(const Texture &attachment, bool own = false);
    void addAttachment(const Renderbuffer &attachment, bool own = false);
    void setDepthAttachment(const Texture &attachment, bool own = false);
    void setDepthAttachment(const Renderbuffer &attachment, bool own = false);

    void clear();
    void clearDepth();
    void check();

    static Framebuffer createDefault();
    static Framebuffer create();
};

} // namespace Engine