#pragma once

#include "detail/raii.h"

namespace corona::modules::imgui
{
  class window : public detail::scoped_template<window, true>
  {
   public:
    enum class flags
    {
      empty                         = 0,
      no_title_bar                  = 1 << 0,    ///< Disable title-bar
      no_resize                     = 1 << 1,    ///< Disable user resizing with the lower-right grip
      no_move                       = 1 << 2,    ///< Disable user moving the window
      no_scrollbar                  = 1 << 3,    ///< Disable scrollbars (window can still scroll with mouse or programmatically)
      no_scroll_with_mouse          = 1 << 4,    ///< Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
      no_collapse                   = 1 << 5,    ///< Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
      always_auto_resize            = 1 << 6,    ///< Resize every window to its content every frame
      no_background                 = 1 << 7,    ///< Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
      no_saved_settings             = 1 << 8,    ///< Never load/save settings in .ini file
      no_mouse_inputs               = 1 << 9,    ///< Disable catching mouse, hovering test with pass through.
      menu_bar                      = 1 << 10,   ///< Has a menu-bar
      horizontal_scroll_bar         = 1 << 11,   ///< Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
      no_focus_on_appearing         = 1 << 12,   ///< Disable taking focus when transitioning from hidden to visible state
      no_bring_to_front_on_focus    = 1 << 13,   ///< Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
      always_vertical_scrollbar     = 1 << 14,   ///< Always show vertical scrollbar (even if ContentSize.y < Size.y)
      always_horizontal_scrollbar   = 1 << 15,   ///< Always show horizontal scrollbar (even if ContentSize.x < Size.x)
      no_nav_inputs                 = 1 << 16,   ///< No gamepad/keyboard navigation within the window
      no_nav_focus                  = 1 << 17,   ///< No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
      unsaved_document              = 1 << 18,   ///< Append '*' to title without affecting the ID, as a convenience to avoid using the ### fragment identifier.
      no_nav                        = no_nav_inputs | no_nav_focus,
      no_decoration                 = no_title_bar | no_resize | no_move | no_scrollbar | no_collapse,
      no_inputs                     = no_mouse_inputs | no_nav_inputs | no_nav_focus,
    };

    explicit window(std::string_view title, bool* open = nullptr, flags f = flags::empty) noexcept
      : detail::scoped_template<window, true>(extern_::imgui::Begin(title.data(), open, static_cast<ImGuiWindowFlags_>(f)))
    {}

    static auto dtor() noexcept -> void { extern_::imgui::End(); }
  };

  struct child_frame : public detail::scoped_template<child_frame, true>
  {
    enum class flags
    {
      empty                           = 0,
      border                          = 1 << 0, ///< Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason)
      always_use_window_padding       = 1 << 1, ///< Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
      resize_x                        = 1 << 2, ///< Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags)
      resize_y                        = 1 << 3, ///< Allow resize from bottom border (layout direction).
      auto_resize_x                   = 1 << 4, ///< Enable auto-resizing width.
      auto_resize_y                   = 1 << 5, ///< Enable auto-resizing height.
      always_auto_resize              = 1 << 6, ///< Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
      frame_style                     = 1 << 7, ///< Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of WindowPadding.
      nav_flattened                   = 1 << 8  ///< Allow gamepad/keyboard navigation to cross over parent border to this child or between sibling child windows.
    };

    explicit child_frame(
      std::string_view title,
      vec2 const& size = detail::vec2_zero,
      window::flags wf = window::flags::empty,
      flags f = flags::empty
    ) noexcept
      : detail::scoped_template<child_frame, true>(
          extern_::imgui::BeginChild(
            title.data(),
            size,
            static_cast<ImGuiWindowFlags_>(wf),
            static_cast<ImGuiChildFlags_>(f)
          )
        )
    {}

    static auto dtor() noexcept -> void { extern_::imgui::EndChild(); }
  };

  [[nodiscard]] inline auto operator|(window::flags const lhs, window::flags const rhs) -> window::flags {
    return static_cast<window::flags>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
  }

  [[nodiscard]] inline auto operator|(child_frame::flags const lhs, child_frame::flags const rhs) -> child_frame::flags {
    return static_cast<child_frame::flags>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
  }
} // namespace corona::modules::imgui