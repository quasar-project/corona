#pragma once

#include <functional>
#include <qdebugenv/rhi/class_renderer.h>

namespace qdebugenv
{
  class QDE_API CImmediateGuiBridge
  {
   public:
    using FrameFunction = std::function<void()>;

    CImmediateGuiBridge();
    ~CImmediateGuiBridge();
    CImmediateGuiBridge(const CImmediateGuiBridge&) = delete;
    CImmediateGuiBridge(CImmediateGuiBridge&&) = default;
    auto operator=(const CImmediateGuiBridge&) -> CImmediateGuiBridge& = delete;
    auto operator=(CImmediateGuiBridge&&) -> CImmediateGuiBridge& = default;

    auto next_frame(
      ::QSizeF const& logical_output_size,
      f32 dpr,
      ::QPointF const& logical_offset,
      FrameFunction frame_fn
    ) -> void;

    auto sync_renderer(CRhiRenderer* renderer) -> void;
    auto process_event(::QEvent* event) -> bool;
    auto rebuild_font_atlas() -> void;
    auto rebuild_font_atlas_with_font(std::string_view filename);

   private:
    auto swap_ctx() -> void;

    void* ctx_;
    CRhiRenderer::StaticRenderData sf_;
    CRhiRenderer::FrameRenderData f_;
    Qt::MouseButtons pressed_mouse_buttons_;
  };
} // namespace qdebugenv