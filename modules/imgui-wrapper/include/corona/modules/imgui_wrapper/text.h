#pragma once

#include "detail/export.h"

namespace corona::modules::imgui
{
  struct text
  {
    enum flavor : char
    {
      bullet
    };

    template <typename... Args>
    inline explicit text(fmt::string_view const format, Args&&... args) {
      extern_::imgui::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    inline text(flavor const f, fmt::string_view const format, Args&&... args) {
      switch(f) {
        case bullet:
          extern_::imgui::Bullet();
          extern_::imgui::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
          break;
      }
    }

    template <typename... Args>
    inline text(vec4 const& col, std::string_view const format, Args&&... args) {
      extern_::imgui::PushStyleColor(ImGuiCol_Text, col);
      extern_::imgui::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
      extern_::imgui::PopStyleColor();
    }

    template <typename... Args>
    inline text(flavor const f, vec4 const& col, std::string_view const format, Args&&... args) {
      switch(f) {
        case bullet:
          extern_::imgui::PushStyleColor(ImGuiCol_Text, col);
          extern_::imgui::Bullet();
          extern_::imgui::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
          extern_::imgui::PopStyleColor();
          break;
      }
    }
  };

  inline auto same_line() -> void { extern_::imgui::SameLine(); }
  inline auto separator() -> void { extern_::imgui::Separator(); }

  struct table_entry
  {
    template <typename... Args>
    inline table_entry(
      vec4 const& col,
      std::string_view const name,
      std::string_view const format,
      Args&&... args
    ) {
      text(text::bullet, fmt::format("{:<40}", name));
      same_line();
      text(col, format, std::forward<Args>(args)...);
    }
  };
} // namespace corona::modules::imgui