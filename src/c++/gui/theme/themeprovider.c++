//
// Created by whs31 on 09.01.2024.
//

#include "themeprovider.h"
#include <filesystem>
#include <fstream>
#include <ranges>
#include <toml++/toml.h>

using namespace std;

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
    llwarn("invalid palette type: {}", str);
    return PaletteType::Light;
  }

  ThemeProvider::ThemeProvider(const string_view folder, const string_view theme_name, const PaletteType palette_type)
    : m_folder((filesystem::current_path() / folder).string())
    , m_theme_name(theme_name)
    , m_palette_type(palette_type)
  {}

  void ThemeProvider::refresh()
  {
    ensure(verify_path(this->folder()), "theme folder does not exist");
    ensure(not this->theme_name().empty(), "theme name is empty");
    this->m_names = find_theme_names(m_folder)
      .map_error([](const string& err) { llerror("theme provider error: {}", err); })
      .value_or(unordered_map<string, string>());
    ensure(not this->m_names.empty(), "no valid themes found");
    if(not this->m_names.contains(string(this->theme_name())))
    {
      llwarn("theme '{}' not found in folder '{}'", this->theme_name(), this->folder());
      llwarn("themes available: {}", fmt::join(this->m_names | ranges::views::values, ", "));
      llwarn("defaulting to '{}'", this->m_names.begin()->first);
      this->set_theme_name(this->m_names.begin()->first);
    }
    m_palette.clear();
    //m_palette = load_palette_from_file(m_folder, m_theme_name);
  }

  void ThemeProvider::load() { this->refresh(); }

  auto ThemeProvider::theme_name() const -> string_view { return this->m_theme_name; }
  void ThemeProvider::set_theme_name(string_view theme_name)
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

  auto ThemeProvider::find_theme_names(string_view folder) -> expected<unordered_map<string, string>, string>
  {
    lldebug("finding theme names in ...{}", folder.substr(folder.find_last_of("\\/") + 1));
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
        llwarn("failed to parse theme file {}, reason: {}", file.path().filename().string(), err.what());
        continue;
      }
      const auto name = table["meta"]["name"].value<string>();
      if(not name.has_value())
      {
        llwarn("failed to get name from theme file {}", file.path().filename().string());
        continue;
      }

      result.emplace(name.value(), file.path().filename().string());
    }
    if(result.empty())
      return unexpected<string>("no themes found in folder");
    for(const auto & [name, path] : result)
      lldebug("  {} -> {}", name, path);
    return result;
  }

  auto ThemeProvider::load_palette_from_file(string_view folder, string_view theme_name) -> expected<ThemeProvider::PaletteSet, string>
  {

  }
}
