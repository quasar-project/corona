#include <corona-standalone/gui/theme/values.hh>

#include <sstream>
#include <magic_enum/magic_enum.hpp>
#include <toml++/toml.h>

using std::string;
using namespace std::literals;
namespace me = magic_enum;

namespace corona::standalone::gui::theme
{
  static const auto ERRC = QString("#ff00dc");

  Palette::Palette() noexcept = default;
  Palette::Palette(Mode const m) noexcept
    : rosewater(m == Mode::Light ? "#dc8a78" : "#f2d5cf")
    , flamingo(m == Mode::Light ? "#dd7878" : "#eebebe")
    , pink(m == Mode::Light ? "#ea76cb" : "#f4b8e4")
    , mauve(m == Mode::Light ? "#8839ef" : "#ca9ee6")
    , red(m == Mode::Light ? "#d20f39" : "#e78284")
    , maroon(m == Mode::Light ? "#e64553" : "#ea999c")
    , peach(m == Mode::Light ? "#fe640b" : "#ef9f76")
    , yellow(m == Mode::Light ? "#df8e1d" : "#e5c890")
    , green(m == Mode::Light ? "#a6d189" : "#40a02b")
    , teal(m == Mode::Light ? "#179299" : "#81c8be")
    , sky(m == Mode::Light ? "#04a5e5" : "#99d1db")
    , sapphire(m == Mode::Light ? "#209fb5" : "#85c1dc")
    , blue(m == Mode::Light ? "#1e66f5" : "#8caaee")
    , lavender(m == Mode::Light ? "#7287fd" : "#babbf1")
    , grey0(m == Mode::Light ? "#d6d7d7" : "#4b4d56")
    , text(m == Mode::Light ? "#4c4f69" : "#c6d0f5")
    , subtext1(m == Mode::Light ? "#5c5f77" : "#b5bfe2")
    , subtext0(m == Mode::Light ? "#6c6f85" : "#a5adce")
    , overlay2(m == Mode::Light ? "#7c7f93" : "#949cbb")
    , overlay1(m == Mode::Light ? "#8c8fa1" : "#838ba7")
    , overlay0(m == Mode::Light ? "#9ca0b0" : "#737994")
    , surface2(m == Mode::Light ? "#acb0be" : "#626880")
    , surface1(m == Mode::Light ? "#bcc0cc" : "#51576d")
    , surface0(m == Mode::Light ? "#ccd0da" : "#414559")
    , base2(m == Mode::Light ? "#eff1f5" : "#303446")
    , base1(m == Mode::Light ? "#e6e9ef" : "#292c3c")
    , base0(m == Mode::Light ? "#dce0e8" : "#232634")
    , black("#000000")
    , white("#ffffff")
    , transparent("#00000000")
  {}

  auto Palette::invalid() -> Palette const& {
    static auto res = Palette();
    res.rosewater = ERRC;
    res.flamingo = ERRC;
    res.pink = ERRC;
    res.mauve = ERRC;
    res.red = ERRC;
    res.maroon = ERRC;
    res.peach = ERRC;
    res.yellow = ERRC;
    res.green = ERRC;
    res.teal = ERRC;
    res.sky = ERRC;
    res.sapphire = ERRC;
    res.blue = ERRC;
    res.lavender = ERRC;
    res.grey0 = ERRC;
    res.text = ERRC;
    res.subtext1 = ERRC;
    res.subtext0 = ERRC;
    res.overlay2 = ERRC;
    res.overlay1 = ERRC;
    res.overlay0 = ERRC;
    res.surface2 = ERRC;
    res.surface1 = ERRC;
    res.surface0 = ERRC;
    res.base2 = ERRC;
    res.base1 = ERRC;
    res.base0 = ERRC;

    return res;
  }

  auto ThemeData::palette_for(Mode mode) const -> Palette const& {
    switch(mode) {
      case Mode::Light:
        if(not this->palette.light.has_value())
          throw std::runtime_error("requested mode `light` but no light palette found for theme: `" + this->meta.name + "`");
        return *this->palette.light;
      case Mode::Dark:
        if(not this->palette.dark.has_value())
          throw std::runtime_error("requested mode `dark` but no dark palette found for theme: `" + this->meta.name + "`");
        return *this->palette.dark;
      [[unlikely]] default: fl::unreachable();
    }
  }
  auto ThemeData::has_pallete_for(Mode mode) const -> bool {
    try {
      std::ignore = this->palette_for(mode);
      return true;
    } catch(...) {
      return false;
    }
  }
} // namespace corona::standalone::gui::theme

template <>
auto fl::serialization::serialize<fl::serialization::format::toml>(corona::standalone::gui::theme::ThemeData const& value)
  -> std::basic_string<char>
{
  auto serialize_palette = [](corona::standalone::gui::theme::Palette const& palette)
    -> toml::table
  {
    return toml::table{
      {"rosewater", palette.rosewater.toStdString()},
      {"flamingo", palette.flamingo.toStdString()},
      {"pink", palette.pink.toStdString()},
      {"mauve", palette.mauve.toStdString()},
      {"red", palette.red.toStdString()},
      {"maroon", palette.maroon.toStdString()},
      {"peach", palette.peach.toStdString()},
      {"yellow", palette.yellow.toStdString()},
      {"green", palette.green.toStdString()},
      {"teal", palette.teal.toStdString()},
      {"sky", palette.sky.toStdString()},
      {"sapphire", palette.sapphire.toStdString()},
      {"blue", palette.blue.toStdString()},
      {"lavender", palette.lavender.toStdString()},
      {"grey0", palette.grey0.toStdString()},
      {"text", palette.text.toStdString()},
      {"subtext1", palette.subtext1.toStdString()},
      {"subtext0", palette.subtext0.toStdString()},
      {"overlay2", palette.overlay2.toStdString()},
      {"overlay1", palette.overlay1.toStdString()},
      {"overlay0", palette.overlay0.toStdString()},
      {"surface2", palette.surface2.toStdString()},
      {"surface1", palette.surface1.toStdString()},
      {"surface0", palette.surface0.toStdString()},
      {"base2", palette.base2.toStdString()},
      {"base1", palette.base1.toStdString()},
      {"base0", palette.base0.toStdString()}
    };
  };
  auto p = toml::table{};

  if(not value.palette.dark and not value.palette.light)
    throw fl::serialization::serialization_error(serialization::format::toml);
  if(value.palette.light)
    p.insert("light", serialize_palette(value.palette.light.value()));
  if(value.palette.dark)
    p.insert("dark", serialize_palette(value.palette.dark.value()));

  const auto out = toml::table{
    {"meta", toml::table{
               {"name", value.meta.name},
               {"type", value.meta.type}
             }},
    {"font", toml::table{
               {"family", value.font.family},
               {"pixelsize", value.font.pixelsize},
               {"pointsize", value.font.pointsize},
               {"weight", value.font.weight},
             }},
    {"palette", p}
  };
  return (std::stringstream() << out).str();
}

template <>
auto fl::serialization::deserialize<fl::serialization::format::toml>(std::basic_string<char> const& value)
  -> corona::standalone::gui::theme::ThemeData
{
  auto res = corona::standalone::gui::theme::ThemeData{};
  auto deserialize_palette = [](const toml::node_view<toml::node> node) -> corona::standalone::gui::theme::Palette
  {
    auto l = corona::standalone::gui::theme::Palette();
    l.rosewater = QString::fromStdString(node["rosewater"].value<string>().value());
    l.flamingo = QString::fromStdString(node["flamingo"].value<string>().value());
    l.pink = QString::fromStdString(node["pink"].value<string>().value());
    l.mauve = QString::fromStdString(node["mauve"].value<string>().value());
    l.red = QString::fromStdString(node["red"].value<string>().value());
    l.maroon = QString::fromStdString(node["maroon"].value<string>().value());
    l.peach = QString::fromStdString(node["peach"].value<string>().value());
    l.yellow = QString::fromStdString(node["yellow"].value<string>().value());
    l.green = QString::fromStdString(node["green"].value<string>().value());
    l.teal = QString::fromStdString(node["teal"].value<string>().value());
    l.sky = QString::fromStdString(node["sky"].value<string>().value());
    l.sapphire = QString::fromStdString(node["sapphire"].value<string>().value());
    l.blue = QString::fromStdString(node["blue"].value<string>().value());
    l.lavender = QString::fromStdString(node["lavender"].value<string>().value());
    l.grey0 = QString::fromStdString(node["grey0"].value<string>().value());
    l.text = QString::fromStdString(node["text"].value<string>().value());
    l.subtext1 = QString::fromStdString(node["subtext1"].value<string>().value());
    l.subtext0 = QString::fromStdString(node["subtext0"].value<string>().value());
    l.overlay2 = QString::fromStdString(node["overlay2"].value<string>().value());
    l.overlay1 = QString::fromStdString(node["overlay1"].value<string>().value());
    l.overlay0 = QString::fromStdString(node["overlay0"].value<string>().value());
    l.surface2 = QString::fromStdString(node["surface2"].value<string>().value());
    l.surface1 = QString::fromStdString(node["surface1"].value<string>().value());
    l.surface0 = QString::fromStdString(node["surface0"].value<string>().value());
    l.base2 = QString::fromStdString(node["base2"].value<string>().value());
    l.base1 = QString::fromStdString(node["base1"].value<string>().value());
    l.base0 = QString::fromStdString(node["base0"].value<string>().value());
    return l;
  };

  auto in = toml::table();
  try {
    in = toml::parse(value);
  } catch(const toml::parse_error& err) {
    throw fl::serialization::serialization_error(serialization::format::toml);
  }
  try {
    res.meta.name = in["meta"]["name"].value<string>().value();
    res.meta.type = in["meta"]["type"].value<string>().value();
    res.font.family = in["font"]["family"].value<string>().value();
    res.font.pixelsize = in["font"]["pixelsize"].value<i64>().value();
    res.font.pointsize = in["font"]["pointsize"].value<i64>().value();
    res.font.weight = in["font"]["weight"].value<i64>().value();
    if(not in["palette"]["light"].is_table() and not in["palette"]["dark"].is_table())
      throw fl::serialization::serialization_error(serialization::format::toml);
    if(const auto light = in["palette"]["light"]; light.is_table())
      res.palette.light = deserialize_palette(light);
    if(const auto dark = in["palette"]["dark"]; dark.is_table())
      res.palette.dark = deserialize_palette(dark);
  } catch(std::exception const& err) {
    throw fl::serialization::serialization_error(serialization::format::toml);
  }
  return res;
}
