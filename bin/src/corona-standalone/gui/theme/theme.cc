#include <corona-standalone/gui/theme/theme.hh>

#include <fstream>
#include <sstream>
#include <rfl.hpp>
#include <rfl/yaml.hpp>
#include <magic_enum/magic_enum.hpp>

using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::istreambuf_iterator;
using namespace std::literals;
namespace me = magic_enum;

namespace corona::standalone::gui::theme
{
  constexpr auto ROOT_SUBDIRECTORY = "gui"sv;
  constexpr auto THEMES_SUBDIRECTORY = "themes"sv;
  constexpr auto DEFAULT_THEME_FILENAME = "default-autogen.toml"sv;

  Theme::Theme(fl::filesystem::application_dirs const& location)
    : cfg_(fl::configuration_file<fl::serialization::format::yaml, ThemeConfiguration>(
      "theme_config.yml",
      location[floppy::filesystem::application_dirs::dir::config] / ROOT_SUBDIRECTORY,
      fl::saving_policy::autosave
    ))
    , folder_(location[floppy::filesystem::application_dirs::dir::data] / THEMES_SUBDIRECTORY)
  {
    if(not this->cfg_)
      llog::error("failed to load theme configuration");
    this->cfg_.load();
    if(not exists(this->folder_)) {
      llog::trace("created missing theme folder: {}", this->folder_.generic_string());
      create_directories(this->folder_);
    }
    llog::debug("assuming theme pwd: {}", this->folder_.generic_string());
    try {
      this->load();
    } catch(std::exception const& e) {
      llog::error("fatal error during theme initialization: {}", e.what());
    }
  }
  auto Theme::folder() const -> fs::path const& { return this->folder_; }
  auto Theme::theme_name() const -> string_view { return this->cfg_().active; }
  auto Theme::mode() const -> Mode { return this->cfg_().mode; }
  auto Theme::active() const -> ThemeData const& { return this->active_; }
  auto Theme::palette() const -> Palette const& {
    if(not this->active().has_pallete_for(this->mode()))
      return Palette::invalid();
    return this->active().palette_for(this->mode());
  }
  auto Theme::set_theme_name(string_view name) -> void {
    this->cfg_().active = name;
    this->load();
  }

  auto Theme::set_mode(Mode mode) -> void {
    if(not this->active().has_pallete_for(mode)) {
      llog::warn("requested mode `{}` but no palette found for theme: `{}`", me::enum_name(mode), this->active().meta.name);
      return;
    }
    this->cfg_().mode = mode;
  }

  auto Theme::swap_mode() -> void {
    switch(this->mode()) {
      case Mode::Light:
        this->set_mode(Mode::Dark);
        break;
      case Mode::Dark:
        this->set_mode(Mode::Light);
        break;
      [[unlikely]] default: fl::unreachable();
    }
  }

  auto Theme::load() -> void {
    auto active_file = [this]() -> option<fs::path> {
      for(const auto& entry : fs::directory_iterator(this->folder())) {
        if(not entry.is_regular_file() or entry.path().extension() != ".toml")
          continue;
        const auto contents = Theme::read_theme(entry.path());
        if(not contents) {
          llog::warn("error reading theme file ({})", entry.path().filename().string());
          continue;
        }
        if(contents.value().meta.name == this->cfg_().active)
          return entry.path();
      }
      return none;
    }();

    if(not active_file) {
      llog::warn("theme with name `{}` is not found. falling back to default theme", this->cfg_().active);
      try {
        active_file = this->emplace_default();
      } catch(std::exception const& e) {
        llog::error("failed to create default theme: {}", e.what());
        return;
      }
    }

    if(const auto active = Theme::read_theme(*active_file); not active)
      throw std::runtime_error("failed to load theme: "s + active_file->filename().string());
    else
      this->active_ = active.value();
    llog::info("loaded theme: `{}`", this->active_.meta.name);
    if(not this->active().has_pallete_for(this->mode()))
      this->swap_mode();
  }
  auto Theme::emplace_default() -> fs::path {
    auto const contents = fl::serialization::serialize<fl::serialization::format::toml>(ThemeData());
    auto path = this->folder() / DEFAULT_THEME_FILENAME;
    ofstream(path) << contents;
    if(not exists(path))
      throw std::runtime_error("failed to create default theme: "s + path.generic_string());
    this->cfg_().active = "default";
    llog::debug("created default theme: {}", path.generic_string());
    return path;
  }

  auto Theme::read_theme(fs::path const& path) -> option<ThemeData> {
    auto stream = ifstream(path);
    if(not stream) {
      llog::warn("failed to open theme file: {}", path.generic_string());
      return none;
    }
    auto contents = string(istreambuf_iterator<char>{stream}, istreambuf_iterator<char>{});
    if(not stream) {
      llog::warn("failed to read theme file: {}", path.generic_string());
      return none;
    }
    try {
      return fl::serialization::deserialize<fl::serialization::format::toml, ThemeData>(contents);
    } catch(std::exception const& e) {
      llog::warn("failed to deserialize theme file: {}, reason: {}", path.string(), e.what());
      return none;
    }
  }
} // namespace corona::standalone::gui::theme

namespace s = fl::serialization;
namespace csgt = corona::standalone::gui::theme;

template <> auto s::serialize<s::format::yaml>(csgt::ThemeConfiguration const& value) -> std::basic_string<char> {
  return rfl::yaml::write(value);
}

template <> auto s::deserialize<s::format::yaml>(std::basic_string<char> const& value) -> csgt::ThemeConfiguration {
  return rfl::yaml::read<corona::standalone::gui::theme::ThemeConfiguration>(value).value();
}