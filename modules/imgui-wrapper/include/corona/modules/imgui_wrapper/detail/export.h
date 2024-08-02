#pragma once

#include <floppy/floppy.h>
#include <corona/modules/extern/imgui/imgui.h>

namespace corona::modules::imgui
{
  using namespace fl::types;

  using vec2 = ImVec2;
  using vec4 = ImVec4;
} // namespace corona::modules::imgui

inline constexpr auto operator==(corona::modules::imgui::vec2 const& lhs, corona::modules::imgui::vec2 const& rhs) -> bool {
  return lhs.x == rhs.x and lhs.y == rhs.y;
}

inline constexpr auto operator==(corona::modules::imgui::vec4 const& lhs, corona::modules::imgui::vec4 const& rhs) -> bool {
  return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z and lhs.w == rhs.w;
}