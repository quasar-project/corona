#pragma once

#include <qdebugenv/imgui/detail.h>

namespace qdebugenv::imgui
{
  using vec4 = ImVec4;

  inline constexpr auto operator==(qdebugenv::imgui::vec4 const& lhs, qdebugenv::imgui::vec4 const& rhs) -> bool {
    return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z and lhs.w == rhs.w;
  }

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
    std::array<vec4, static_cast<usize>(color::_N)> m_colors;
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

    explicit style(palette const& p, style::roundings const& r) noexcept {
      using enum palette::color;

      auto& ref = sys::GetStyle();
      ref.TabRounding = r.tab;
      ref.ScrollbarRounding = r.scrollbar;
      ref.WindowRounding = r.window;
      ref.GrabRounding = r.grab;
      ref.FrameRounding = r.frame;
      ref.PopupRounding = r.popup;
      ref.ChildRounding = r.child;

      auto& colors = ref.Colors;
      colors[ImGuiCol_WindowBg] = p[crust];
      colors[ImGuiCol_MenuBarBg] = p[surface0];
      colors[ImGuiCol_Border] = p[dark_purple];
      colors[ImGuiCol_BorderShadow] = p[black];
      colors[ImGuiCol_Text] = p[foreground];
      colors[ImGuiCol_TextDisabled] = p[dark_foreground];
      colors[ImGuiCol_Header] = p[mantle];
      colors[ImGuiCol_HeaderHovered] = p[surface1];
      colors[ImGuiCol_HeaderActive] = p[surface0];
      colors[ImGuiCol_Button] = p[base];
      colors[ImGuiCol_ButtonHovered] = p[surface1];
      colors[ImGuiCol_ButtonActive] = p[surface0];
      colors[ImGuiCol_CheckMark] = p[purple];
      colors[ImGuiCol_PopupBg] = palette::alpha(p[crust], 92);
      colors[ImGuiCol_SliderGrab] = palette::alpha(p[dark_purple], 54);
      colors[ImGuiCol_SliderGrabActive] = palette::alpha(p[purple], 54);
      colors[ImGuiCol_FrameBg] = p[mantle];
      colors[ImGuiCol_FrameBgHovered] = p[surface1];
      colors[ImGuiCol_FrameBgActive] = p[surface0];
      colors[ImGuiCol_Tab] = p[surface0];
      colors[ImGuiCol_TabHovered] = p[overlay0];
      colors[ImGuiCol_TabActive] = p[surface2];
      colors[ImGuiCol_TabUnfocused] = p[surface0];
      colors[ImGuiCol_TabUnfocusedActive] = p[surface0];
      colors[ImGuiCol_TitleBg] = p[surface0];
      colors[ImGuiCol_TitleBgActive] = p[surface0];
      colors[ImGuiCol_TitleBgCollapsed] = p[surface0];
      colors[ImGuiCol_ScrollbarBg] = p[crust];
      colors[ImGuiCol_ScrollbarGrab] = p[surface0];
      colors[ImGuiCol_ScrollbarGrabHovered] = p[surface1];
      colors[ImGuiCol_ScrollbarGrabActive] = p[overlay0];
      colors[ImGuiCol_Separator] = p[purple];
      colors[ImGuiCol_SeparatorHovered] = p[purple];
      colors[ImGuiCol_SeparatorActive] = p[light_purple];
      colors[ImGuiCol_ResizeGrip] = palette::alpha(p[dark_purple], 29);
      colors[ImGuiCol_ResizeGripHovered] = palette::alpha(p[purple], 29);
      colors[ImGuiCol_ResizeGripActive] = palette::alpha(p[light_purple], 29);
      colors[ImGuiCol_DockingPreview] = p[purple];
      colors[ImGuiCol_PlotHistogram] = p[cyan];
      colors[ImGuiCol_PlotHistogramHovered] = p[foreground];
      colors[ImGuiCol_PlotLines] = p[yellow];
      colors[ImGuiCol_PlotLinesHovered] = p[orange];
    }
  };

  static_assert(palette::vec4_from_hex("#7EA3CE") == palette::vec4_from_rgb(126, 163, 206), "HEX to RGB conversion failed");

  constexpr inline auto default_palette = palette(palette::flavor::dracula);
} // namespace qdebugenv::imgui

using qdebugenv::imgui::operator==;
