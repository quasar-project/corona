//
// Created by whs31 on 09.01.2024.
//

#include <array>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <toml++/toml.h>
#include <gui/theme/themeprovider.h>

using namespace std;

constexpr array<string_view, 26> ThemeTOMLColorKeys = {
  "rosewater",
  "flamingo",
  "pink",
  "mauve",
  "red",
  "maroon",
  "peach",
  "yellow",
  "green",
  "teal",
  "sky",
  "sapphire",
  "blue",
  "lavender",
  "text",
  "subtext1",
  "subtext0",
  "overlay2",
  "overlay1",
  "overlay0",
  "surface2",
  "surface1",
  "surface0",
  "base",
  "mantle",
  "crust"
};

constexpr array<string_view, 4> ThemeTOMLConfigurationKeys = {
  "foreground",
  "background",
  "accent",
  "main"
};

namespace gui::theme
{
  auto verify_path(const string_view path) -> bool
  {
    return filesystem::exists(path) and filesystem::is_directory(path);
  }

  auto ThemeProvider::palette_from_string_view(string_view str) -> PaletteType
  {
    if(str == "light")
      return PaletteType::Light;
    if(str == "dark")
      return PaletteType::Dark;
    llog::warn("invalid palette type: {}", str);
    return PaletteType::Light;
  }

  ThemeProvider::ThemeProvider(const string_view folder, const string_view theme_name, const PaletteType palette_type)
    : m_folder((filesystem::current_path() / folder).string())
    , m_theme_name(theme_name)
    , m_palette_type(palette_type)
  {}

  void ThemeProvider::refresh()
  {
    if(not verify_path(this->folder())) {
      llog::error("theme folder does not exist: {}", this->folder());
      return;
    }
    if(this->theme_name().empty()) {
      llog::error("theme name is empty");
      return;
    }
    this->m_names = find_theme_names(m_folder)
      .map_error([](const string& err) { llog::error("theme provider error: {}", err); })
      .value_or(unordered_map<string, string>());
    if(this->m_names.empty()) {
      llog::error("no valid themes found");
      return;
    }
    if(not this->m_names.contains(string(this->theme_name())))
    {
      llog::warn("theme '{}' not found in folder '{}'", this->theme_name(), this->folder());
      llog::warn("themes available: {}", fmt::join(this->m_names | ranges::views::values, ", "));
      llog::warn("defaulting to '{}'", this->m_names.begin()->first);
      this->set_theme_name(this->m_names.begin()->first);
    }
    this->m_palette.clear();
    this->m_palette = load_palette_from_file(m_folder, m_theme_name)
      .map_error([](const string& err) { llog::error("theme provider error: {}", err); })
      .value_or(unordered_map<PaletteType, unordered_map<string, string>>{});
    if(this->m_palette.empty())
    {
      llog::error("failed to load palette");
      return;
    }
  }

  void ThemeProvider::load() { this->refresh(); }

  auto ThemeProvider::theme_name() const -> string_view { return this->m_theme_name; }
  void ThemeProvider::set_theme_name(const string_view theme_name)
  {
    if(this->m_theme_name != theme_name)
    {
      this->m_theme_name = theme_name;
      this->refresh();
    }
  }

  auto ThemeProvider::palette_type() const -> PaletteType { return this->m_palette_type; }
  void ThemeProvider::set_palette_type(PaletteType palette_type) { this->m_palette_type = palette_type; }
  auto ThemeProvider::folder() const -> string_view { return this->m_folder; }
  void ThemeProvider::set_folder(string_view folder)
  {
    if(this->m_folder != folder)
    {
      this->m_folder = folder;
      this->refresh();
    }
  }

  auto ThemeProvider::color(const string_view name) const -> expected<string, string>
  {
    if(not this->m_palette.contains(this->m_palette_type))
      return leaf::Err(string("palette not found"));
    if(not this->m_palette.at(this->m_palette_type).contains(string(name)))
      return leaf::Err(string("color not found: ") + string(name));
    return this->m_palette.at(this->m_palette_type).at(string(name));
  }

  auto ThemeProvider::find_theme_names(string_view folder) -> expected<unordered_map<string, string>, string>
  {
    llog::debug("finding theme names in ...{}", folder.substr(folder.find_last_of("\\/") + 1));
    unordered_map<string, string> result;
    for(const auto& file : filesystem::directory_iterator(folder))
    {
      if(not file.is_regular_file() or file.path().extension() != ".toml")
        continue;
      ifstream stream(file.path());
      const string contents((istreambuf_iterator(stream)), istreambuf_iterator<char>());
      toml::parse_result table;
      try { table = toml::parse(contents); }
      catch(const toml::parse_error& err)
      {
        llog::warn("failed to parse theme file {}, reason: {}", file.path().filename().string(), err.what());
        continue;
      }
      const auto name = table["meta"]["name"].value<string>();
      if(not name.has_value())
      {
        llog::warn("failed to get name from theme file {}", file.path().filename().string());
        continue;
      }

      result.emplace(name.value(), file.path().filename().string());
    }
    if(result.empty())
      return leaf::Err<string>("no themes found in folder");
    for(const auto & [name, path] : result)
      llog::debug("  {} -> {}", name, path);
    return result;
  }

  auto ThemeProvider::load_palette_from_file(const string_view folder, const string_view theme_name) const -> expected<ThemeProvider::PaletteSet, string>
  {
    if(not this->m_names.contains(string(theme_name)))
      return leaf::Err(string("theme not found"));

    PaletteSet result;
    const auto path = fmt::format("{}/{}", string(folder), this->m_names.at(string(theme_name)));
    ifstream stream(path);
    const string contents((istreambuf_iterator(stream)), istreambuf_iterator<char>());
    toml::parse_result table;
    try { table = toml::parse(contents); }
    catch(const toml::parse_error& err)
    {
      llog::warn("failed to parse theme file {}, reason: {}", filesystem::path(path).filename().string(), err.what());
      return leaf::Err(string("failed to parse theme file"));
    }

    for(const auto& key : ThemeTOMLColorKeys)
    {
      const auto dark = table["dark"][key].value<string>();
      const auto light = table["light"][key].value<string>();
      if(not dark.has_value() or not light.has_value())
      {
        llog::warn("failed to get color '{}' from theme file {}", key, filesystem::path(path).filename().string());
        continue;
      }
      result[PaletteType::Dark][string(key)] = dark.value();
      result[PaletteType::Light][string(key)] = light.value();
    }

    for(const auto& key : ThemeTOMLConfigurationKeys)
    {
      const auto value = table["primary"][key].value<string>();
      if(not value.has_value())
      {
        llog::warn("failed to get color '{}' from theme file {}", key, filesystem::path(path).filename().string());
        continue;
      }
      const auto& name = value.value();
      for(auto& palette : result | views::values)
      {
        if(not palette.contains(string(name)))
        {
          llog::warn("failed to get primary color '{}' from theme file {}", key, filesystem::path(path).filename().string());
          continue;
        }
        palette[string(key)] = palette[string(name)];
      }
    }

    llog::debug("searhing for color keys in theme file {}: done", filesystem::path(path).filename().string());
    return result;
  }
}
