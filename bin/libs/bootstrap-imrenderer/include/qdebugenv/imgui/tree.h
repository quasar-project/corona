#pragma once

#include <functional>
#include <qdebugenv/imgui/detail.h>

namespace qdebugenv::imgui
{
  enum class tree_node_flags
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

  [[nodiscard]] inline auto operator|(tree_node_flags const lhs, tree_node_flags const rhs) -> tree_node_flags {
    return static_cast<tree_node_flags>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
  }

  struct collapsing_header
  {
    inline collapsing_header(std::string_view const name, tree_node_flags const flags, std::function<void(void)> const& content) {
      if(sys::CollapsingHeader(name.data(), static_cast<ImGuiTreeNodeFlags_>(flags)))
        content();
    }
  };
} // namespace qdebugenv::imgui

using qdebugenv::imgui::operator|;