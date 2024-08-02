#pragma once

#include "detail/raii.h"

namespace corona::modules::imgui
{
  struct collapsing_header : public detail::scoped_template<collapsing_header>
  {
    enum class flags
    {
      none = ImGuiTreeNodeFlags_None,                                      ///< No flags specified. Default.
      selected = ImGuiTreeNodeFlags_Selected,                              ///< Node will be drawn as selected.
      framed = ImGuiTreeNodeFlags_Framed,                                  ///< Node will be drawn inside frame with background.
      allow_overlap = ImGuiTreeNodeFlags_AllowItemOverlap,                 ///< Hit testing to allow subsequent widgets to overlap this one.
      no_tree_push_on_open = ImGuiTreeNodeFlags_NoTreePushOnOpen,          ///< Don't do a TreePush() when open (e.g. for CollapsingHeader)
      no_auto_open_on_log = ImGuiTreeNodeFlags_NoAutoOpenOnLog,            ///< Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
      default_open = ImGuiTreeNodeFlags_DefaultOpen,                       ///< Make tree node default to being open.
      open_on_double_click = ImGuiTreeNodeFlags_OpenOnDoubleClick,         ///< Only opens the tree node if double-clicked.
      open_on_arrow = ImGuiTreeNodeFlags_OpenOnArrow,                      ///< Only opens the tree node if arrow clicked.
      leaf = ImGuiTreeNodeFlags_Leaf,                                      ///< No collapsing, no arrow display
      bullet = ImGuiTreeNodeFlags_Bullet,                                  ///< Display a bullet instead of arrow. <b>IMPORTANT</b>: node can still be marked open/close if you don't set the <tt>leaf</tt> flag!
      frame_padding = ImGuiTreeNodeFlags_FramePadding,                     ///< Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height.
      span_avail_width = ImGuiTreeNodeFlags_SpanAvailWidth,                ///< Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line.
      span_full_width = ImGuiTreeNodeFlags_SpanFullWidth,                  ///< Extend hit box to the left-most and right-most edges (bypass the indented area).
      nav_left_jumps_back_here = ImGuiTreeNodeFlags_NavLeftJumpsBackHere   ///< (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
    };

    explicit collapsing_header(std::string_view label, flags f = flags::none) noexcept
      : detail::scoped_template<collapsing_header>(extern_::imgui::CollapsingHeader(label.data(), static_cast<ImGuiTreeNodeFlags_>(f)))
    {}

    template <typename... Args>
    explicit collapsing_header(flags f, fmt::basic_format_string<Args...> const& fmt, Args&&... args) noexcept
      : detail::scoped_template<collapsing_header>(extern_::imgui::CollapsingHeader(fmt::format(fmt, std::forward<Args>(args)...).c_str(), static_cast<ImGuiTreeNodeFlags_>(f)))
    {}

    static auto dtor() -> void {}
  };

  void consteval enum_flag(collapsing_header::flags);
} // namespace corona::modules::imgui