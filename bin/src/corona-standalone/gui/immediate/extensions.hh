#pragma once

#include <functional>
#include <imgui.h>
#include <floppy/floppy.h>

namespace im = ImGui;
namespace ImGui
{
  using vec4 = ImVec4;
  using vec2 = ImVec2;
  template <typename T> using vector = ImVector<T>;
  using text_buffer = ImGuiTextBuffer;
  using text_filter = ImGuiTextFilter;

  inline constexpr auto operator==(vec4 const& lhs, vec4 const& rhs) -> bool {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
  }

  namespace ext
  {
    using std::function;
    using std::array;
    using std::string_view;
    using namespace fl::types;

    class palette
    {
     public:
      enum class flavor
      {
        dracula
      };

      enum class color
      {
        black,
        dark_background,
        mantle,
        background,
        selection_background,
        surface1,
        surface2,
        currentLine,
        selection,
        dark_foreground,
        foreground,
        comment,
        cyan,
        green,
        orange,
        pink,
        dark_purple,
        purple,
        light_purple,
        red,
        yellow,
        _N,
        crust = dark_background,
        base = background,
        surface0 = selection_background,
        overlay0 = currentLine
      };

      [[nodiscard]] constexpr static auto vec4_from_rgb(usize const r, usize const g, usize const b) -> vec4 {
        return { f32(r) / 255.0f, f32(g) / 255.0f, f32(b) / 255.0f, 1.0f };
      }

      struct invalid_hex_char {};
      [[nodiscard]] constexpr static auto vec4_from_hex(char const* hex) -> vec4 {
        auto constexpr cc = [](char const c) -> i32 {
          return (c >= '0' && c <= '9') ? c - '0' : ((c >= 'a' && c <= 'f') ? c - 'a' + 10
                                                     : (c >= 'A' && c <= 'F') ? c - 'A' + 10
                                                                              : throw invalid_hex_char{} // NOLINT(*-exception-baseclass)
                                                    );
        };
        auto r = (cc(hex[1]) << 4 | cc(hex[2])) & 0xFF;
        auto g = (cc(hex[3]) << 4 | cc(hex[4])) & 0xFF;
        auto b = (cc(hex[5]) << 4 | cc(hex[6])) & 0xFF;
        return palette::vec4_from_rgb(r, g, b);
      }

      [[nodiscard]] constexpr static auto alpha(vec4 const& col, i32 const alpha) -> vec4 {
        return { col.x, col.y, col.z, static_cast<f32>(alpha) / 100.0f };
      }

      constexpr explicit palette([[maybe_unused]] flavor const f)
        : m_colors {
            vec4_from_hex("#000000"),  // black
            vec4_from_hex("#1A1A1A"),  // dark_background
            vec4_from_hex("#21212B"),  // mantle
            vec4_from_hex("#282A36"),  // background
            vec4_from_hex("#292936"),  // selection_background (surface0)
            vec4_from_hex("#303340"),  // surface1
            vec4_from_hex("#333845"),  // surface2
            vec4_from_hex("#44475A"),  // currentLine
            vec4_from_hex("#44475A"),  // selection
            vec4_from_hex("#7F7F7F"),  // dark_foreground
            vec4_from_hex("#F8F8F2"),  // foreground
            vec4_from_hex("#6272A4"),  // comment
            vec4_from_hex("#8BE9FD"),  // cyan
            vec4_from_hex("#50FA7B"),  // green
            vec4_from_hex("#FFB86C"),  // orange
            vec4_from_hex("#FF79C6"),  // pink
            vec4_from_hex("#705E9C"),  // dark_purple
            vec4_from_hex("#BD93F9"),  // purple
            vec4_from_hex("#D694FF"),  // light_purple
            vec4_from_hex("#FF5555"),  // red
            vec4_from_hex("#F1FA8C"),  // yellow
          }
      {}

      [[nodiscard]] constexpr auto operator[](color const col) const -> vec4 const& {
        return this->m_colors[static_cast<usize>(col)];
      }

      [[nodiscard]] constexpr auto retrieve(color const col) const -> vec4 const& { return (*this)[col]; }

     private:
      array<vec4, static_cast<usize>(color::_N)> m_colors;
    };

    class style
    {
     public:
      struct roundings
      {
        f32 tab = 4;
        f32 scrollbar = 9;
        f32 window = 7;
        f32 grab = 3;
        f32 frame = 3;
        f32 popup = 4;
        f32 child = 4;
      };

      explicit style(palette const& p, style::roundings const& r) noexcept;
    };

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
      inline collapsing_header(string_view const name, tree_node_flags const flags, function<void(void)> const& content)
      {
        if(im::CollapsingHeader(name.data(), static_cast<ImGuiTreeNodeFlags_>(flags)))
          content();
      }
    };

    struct text
    {
      enum flavor
      {
        bullet
      };

      template <typename... Args>
      inline text(string_view const format, Args&&... args)
      {
        im::Text(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
      }

      template <typename... Args>
      inline text(flavor const f, string_view const format, Args&&... args)
      {
        switch(f) {
          case bullet:
            im::BulletText(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
            break;
        }
      }

      template <typename... Args>
      inline text(im::vec4 const& col, string_view const format, Args&&... args)
      {
        im::TextColored(col, fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
      }

      template <typename... Args>
      inline text(flavor const f, im::vec4 const& col, string_view const format, Args&&... args)
      {
        switch(f) {
          case bullet:
            im::PushStyleColor(ImGuiCol_Text, col);
            im::BulletText(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
            im::PopStyleColor();
            break;
        }
      }
    };

    inline auto same_line() -> void
    {
      im::SameLine();
    }

    struct debug_field
    {
      template <typename... Args>
      inline debug_field(
        vec4 const& col,
        string_view const name,
        string_view const format,
        Args&&... args
      )
      {
        text(text::bullet, fmt::format("{:<40}", name));
        same_line();
        text(col, format, std::forward<Args>(args)...);
      }
    };

    constexpr inline auto default_palette = palette(palette::flavor::dracula);
  } // namespace ext
} // namespace ImGui

using im::operator==;
static_assert(im::ext::palette::vec4_from_hex("#7EA3CE") == im::ext::palette::vec4_from_rgb(126, 163, 206), "HEX to RGB conversion failed");