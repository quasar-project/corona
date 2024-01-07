#pragma once

#include <string>

namespace launcher
{
  using std::string;

  enum class QtQuickStyle
  {
    Default,
    Material,
    Universal,
    Fusion
  };

  inline auto to_string(const QtQuickStyle style) -> string
  {
    switch(style)
    {
      case QtQuickStyle::Material: return "Material";
      case QtQuickStyle::Universal: return "Universal";
      case QtQuickStyle::Fusion: return "Fusion";
      case QtQuickStyle::Default: [[fallthrough]];
      default: return "Default";
    }
  }

  struct QtQuickOptions
  {
    string entry;
    QtQuickStyle style;
  };
}
