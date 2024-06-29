#pragma once

#include <array>
#include <string_view>

namespace corona::standalone::app
{
  using namespace std::literals;
  inline auto constexpr default_themes = std::array
  {
    ":/app/gruvbox.toml"sv
  };
} // namespace corona::standalone::app