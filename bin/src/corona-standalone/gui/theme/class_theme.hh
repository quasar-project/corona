#pragma once

#include <floppy/directories.h>
#include <corona-standalone/gui/theme/values.hh>

#if not defined(Q_MOC_RUN)
# include <floppy/configuration.h>
#endif

namespace corona::standalone::gui::theme
{
  namespace fs = std::filesystem;

  struct ThemeConfiguration final
  {
    string active = "default";
    Mode mode = Mode::Light;
  };
} // namespace corona::standalone::gui::theme

template <>
auto fl::serialization::serialize<fl::serialization::format::yaml>(corona::standalone::gui::theme::ThemeConfiguration const& value)
  -> std::basic_string<char>;

template <>
auto fl::serialization::deserialize<fl::serialization::format::yaml>(std::basic_string<char> const& value)
  -> corona::standalone::gui::theme::ThemeConfiguration;

namespace corona::standalone::gui::theme
{
  class CTheme final
  {
   public:
    explicit CTheme(fl::application_dirs const& location);

    [[nodiscard]] auto folder() const -> fs::path const&;
    [[nodiscard]] auto theme_name() const -> string_view;
    [[nodiscard]] auto mode() const -> Mode;
    [[nodiscard]] auto active() const -> ThemeData const&;
    [[nodiscard]] auto palette() const -> Palette const&;

    auto set_theme_name(string_view name) -> void;
    auto set_mode(Mode mode) -> void;
    auto swap_mode() -> void;
    auto load() -> void;

   private:
    [[nodiscard]] auto emplace_default() -> fs::path;
    [[nodiscard]] static auto read_theme(fs::path const& path) -> option<ThemeData>;

    fl::configuration_file<fl::serialization::format::yaml, ThemeConfiguration> cfg_;
    fs::path folder_;
    ThemeData active_;
  };
} // namespace corona::standalone::gui::theme