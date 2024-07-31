#pragma once

#include <qglobal.h>
#include <private/qrhi_p.h>
#include <private/qrhinull_p.h>
#include <qdebugenv/detail/export.h>

#if QT_CONFIG(opengl)
# include <private/qrhigles2_p.h>
#endif // QT_CONFIG(opengl)

#if QT_CONFIG(vulkan)
# include <private/qrhivulkan_p.h>
#endif // QT_CONFIG(vulkan)

#ifdef Q_OS_WIN
# include <private/qrhid3d11_p.h>
#endif // Q_OS_WIN

#if defined(Q_OS_MACOS) || defined(Q_OS_IOS)
# include <private/qrhimetal_p.h>
#endif // defined(Q_OS_MACOS) || defined(Q_OS_IOS)

class QEvent;

namespace qdebugenv
{
  class QDE_API CRhiRenderer
  {
   public:
    CRhiRenderer();
    ~CRhiRenderer();
    CRhiRenderer(const CRhiRenderer&) = delete;
    CRhiRenderer(CRhiRenderer&&) = default;
    auto operator=(const CRhiRenderer&) -> CRhiRenderer& = delete;
    auto operator=(CRhiRenderer&&) -> CRhiRenderer& = default;

    struct CommandListBuffer
    {
      u32 offset;
      ::QByteArray data;
    };

    struct DrawCommand
    {
      i32 command_list_buffer_index{};
      void* texture_id{};
      u32 index_offset{};
      u32 element_count{};
      ::QPointF item_pixel_offset{};
      ::QVector4D clip_rect{};
    };

    struct StaticRenderData
    {
      ::QImage font_texture_data;

      [[nodiscard]] auto valid() const -> bool { return not this->font_texture_data.isNull(); }
      auto reset() -> void { this->font_texture_data = ::QImage(); }

      explicit operator bool() const { return this->valid(); }
    };

    struct FrameRenderData
    {
      u32 total_vbuf_size = 0;
      u32 total_ibuf_size = 0;
      ::QVarLengthArray<CommandListBuffer, 4> vbuf;
      ::QVarLengthArray<CommandListBuffer, 16> ibuf;
      ::QVarLengthArray<DrawCommand, 4> draw;
      ::QSize output_pixel_size;
    };

    enum class CustomTextureOwnership : char { Take, None };

    StaticRenderData sf; // NOLINT(*-non-private-member-variables-in-classes)
    FrameRenderData f; // NOLINT(*-non-private-member-variables-in-classes)

    auto prepare(
      ::QRhi* rhi,
      ::QRhiRenderTarget* rt,
      ::QRhiCommandBuffer* cb,
      ::QMatrix4x4 const& mvp,
      f32 opacity = 1.0F,
      f32 hdr_white_level = 0.0F
    ) -> void;

    auto render() -> void;
    auto release_resources() -> void;

    auto register_custom_texture(
      void* id,
      ::QRhiTexture* texture,
      ::QRhiSampler::Filter filter,
      CustomTextureOwnership ownership
    ) -> void;

   private:
    struct Texture
    {
      ::QImage image;
      ::QRhiTexture* tex = nullptr;
      ::QRhiShaderResourceBindings* srb = nullptr;
      ::QRhiSampler::Filter filter = ::QRhiSampler::Filter::Linear;
      bool own_texture = true;
    };

    ::QRhi* rhi_ = nullptr;
    ::QRhiRenderTarget* rt_ = nullptr;
    ::QRhiCommandBuffer* cb_ = nullptr;
    std::unique_ptr<::QRhiBuffer> vbuf_;
    std::unique_ptr<::QRhiBuffer> ibuf_;
    std::unique_ptr<::QRhiBuffer> ubuf_;
    std::unique_ptr<::QRhiGraphicsPipeline> ps_;
    std::vector<u32> render_pass_format_;
    std::unique_ptr<::QRhiSampler> linear_sampler_;
    std::unique_ptr<::QRhiSampler> nearest_sampler_;
    std::unordered_map<void*, Texture> textures_;
  };
} // namespace qdebugenv