#include <corona-standalone/gui/immediate/extensions.hh>

#include <floppy/logging.h>

namespace ImGui::ext
{
  style::style(palette const& p, style::roundings const& r) noexcept
  {
    using enum palette::color;

    auto& ref = im::GetStyle();
    ref.TabRounding = r.tab;
    ref.ScrollbarRounding = r.scrollbar;
    ref.WindowRounding = r.window;
    ref.GrabRounding = r.grab;
    ref.FrameRounding = r.frame;
    ref.PopupRounding = r.popup;
    ref.ChildRounding = r.child;

    auto& colors = ImGui::GetStyle().Colors;
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
    colors[ImGuiCol_PlotHistogram] = p[cyan];
    colors[ImGuiCol_PlotHistogramHovered] = p[foreground];
    colors[ImGuiCol_PlotLines] = p[yellow];
    colors[ImGuiCol_PlotLinesHovered] = p[orange];

    fl::log::trace()("set new imgui style");
  }
} // namespace ImGui::ext