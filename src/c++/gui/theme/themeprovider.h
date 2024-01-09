#pragma once

#include <afx>
#include <unordered_map>
#include <string>

namespace gui::theme
{
  using std::string;
  using std::string_view;
  using std::unordered_map;

  class ThemeProvider
  {
    public:
      enum class PaletteType
      {
        Light,
        Dark
      };

      static auto palette_from_string_view(string_view str) -> PaletteType;

      explicit ThemeProvider(string_view folder, string_view theme_name, PaletteType palette_type);

      void refresh();
      void load();

      [[nodiscard]] auto theme_name() const -> string_view;
      void set_theme_name(string_view theme_name);

      [[nodiscard]] auto palette_type() const -> PaletteType;
      void set_palette_type(PaletteType palette_type);

      [[nodiscard]] auto folder() const -> string_view;
      void set_folder(string_view folder);

      [[nodiscard]] auto color(string_view name) const -> string_view;

    private:
      using Palette = unordered_map<string, string>;
      using PaletteSet = unordered_map<PaletteType, Palette>;

      [[nodiscard]] static auto find_theme_names(string_view folder) -> expected<unordered_map<string, string>, string>;
      [[nodiscard]] auto load_palette_from_file(string_view folder, string_view theme_name) const -> expected<PaletteSet, string>;

    private:
      string m_folder;
      string m_theme_name;
      PaletteType m_palette_type;
      PaletteSet m_palette;
      unordered_map<string, string> m_names;
  };
}