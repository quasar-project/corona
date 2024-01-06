module;

#include <string>

export module launcher.qtquickoptions;

using namespace std;

namespace launcher
{
  export enum class QtQuickStyle
  {
    Default,
    Material,
    Universal,
    Fusion
  };

  export inline auto to_string(const QtQuickStyle style) -> string
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

  export struct QtQuickOptions
  {
    string entry;
    QtQuickStyle style;
  };
}