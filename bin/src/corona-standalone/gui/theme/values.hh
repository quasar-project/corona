#pragma once

#include <qobject.h>
#include <floppy/serialization.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::gui::theme
{
  enum class Mode
  {
    Light,
    Dark
  };

  class Palette
  {
    Q_GADGET
    Q_PROPERTY(QString rosewater MEMBER rosewater CONSTANT FINAL STORED true)
    Q_PROPERTY(QString flamingo MEMBER flamingo CONSTANT FINAL STORED true)
    Q_PROPERTY(QString pink MEMBER pink CONSTANT FINAL STORED true)
    Q_PROPERTY(QString mauve MEMBER mauve CONSTANT FINAL STORED true)
    Q_PROPERTY(QString red MEMBER red CONSTANT FINAL STORED true)
    Q_PROPERTY(QString maroon MEMBER maroon CONSTANT FINAL STORED true)
    Q_PROPERTY(QString peach MEMBER peach CONSTANT FINAL STORED true)
    Q_PROPERTY(QString yellow MEMBER yellow CONSTANT FINAL STORED true)
    Q_PROPERTY(QString green MEMBER green CONSTANT FINAL STORED true)
    Q_PROPERTY(QString teal MEMBER teal CONSTANT FINAL STORED true)
    Q_PROPERTY(QString sky MEMBER sky CONSTANT FINAL STORED true)
    Q_PROPERTY(QString sapphire MEMBER sapphire CONSTANT FINAL STORED true)
    Q_PROPERTY(QString blue MEMBER blue CONSTANT FINAL STORED true)
    Q_PROPERTY(QString lavender MEMBER lavender CONSTANT FINAL STORED true)
    Q_PROPERTY(QString grey0 MEMBER grey0 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString text MEMBER text CONSTANT FINAL STORED true)
    Q_PROPERTY(QString subtext1 MEMBER subtext1 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString subtext0 MEMBER subtext0 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString overlay2 MEMBER overlay2 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString overlay1 MEMBER overlay1 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString overlay0 MEMBER overlay0 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString surface2 MEMBER surface2 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString surface1 MEMBER surface1 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString surface0 MEMBER surface0 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString base2 MEMBER base2 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString base1 MEMBER base1 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString base0 MEMBER base0 CONSTANT FINAL STORED true)
    Q_PROPERTY(QString black MEMBER black CONSTANT FINAL STORED true)
    Q_PROPERTY(QString white MEMBER black CONSTANT FINAL STORED true)
    Q_PROPERTY(QString transparent MEMBER transparent CONSTANT FINAL STORED true)

   public:
    explicit Palette() noexcept;
    explicit Palette(Mode) noexcept;

    [[nodiscard]] static auto invalid() -> Palette const&;

    QString
      rosewater,
      flamingo,
      pink,
      mauve,
      red,
      maroon,
      peach,
      yellow,
      green,
      teal,
      sky,
      sapphire,
      blue,
      lavender,
      grey0,
      text,
      subtext1,
      subtext0,
      overlay2,
      overlay1,
      overlay0,
      surface2,
      surface1,
      surface0,
      base2,
      base1,
      base0,
      black,
      white,
      transparent;
  };

  struct ThemeData final
  {
    [[nodiscard]] auto palette_for(Mode mode) const -> Palette const&;
    [[nodiscard]] auto has_pallete_for(Mode mode) const -> bool;

    struct Meta {
      string name = "default";
      string type = "qml";
    } meta;

    struct Font {
      string family = "Segoe UI";
      i64 pixelsize = 20;
      i64 pointsize = 16;
      i64 weight    = 400;
    } font;

    struct Palettes {
      option<Palette> light = Palette(Mode::Light);
      option<Palette> dark  = Palette(Mode::Dark);
    } palette;
  };
} // namespace corona::standalone::gui::theme

template <>
auto fl::serialization::serialize<fl::serialization::format::toml>(corona::standalone::gui::theme::ThemeData const& value)
  -> std::basic_string<char>;

template <>
auto fl::serialization::deserialize<fl::serialization::format::toml>(std::basic_string<char> const& value)
  -> corona::standalone::gui::theme::ThemeData;

Q_DECLARE_METATYPE(corona::standalone::gui::theme::Palette)
