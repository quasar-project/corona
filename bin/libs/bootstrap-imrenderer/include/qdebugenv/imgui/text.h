#pragma once

#include <qdebugenv/imgui/palette.h>

namespace qdebugenv::imgui
{
  struct text
  {
    enum flavor
    {
      bullet
    };

    template <typename... Args>
    inline text(fmt::string_view const format, Args&&... args) {
      sys::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    inline text(flavor const f, fmt::string_view const format, Args&&... args) {
      switch(f) {
        case bullet:
          sys::Bullet();
          sys::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
          break;
      }
    }

    template <typename... Args>
    inline text(vec4 const& col, std::string_view const format, Args&&... args) {
      sys::PushStyleColor(ImGuiCol_Text, col);
      sys::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
      sys::PopStyleColor();
    }

    template <typename... Args>
    inline text(flavor const f, vec4 const& col, std::string_view const format, Args&&... args) {
      switch(f) {
        case bullet:
          sys::PushStyleColor(ImGuiCol_Text, col);
          sys::Bullet();
          sys::TextUnformatted(fmt::format(fmt::runtime(format), std::forward<Args>(args)...).c_str());
          sys::PopStyleColor();
          break;
      }
    }
  };

  inline auto same_line() -> void { sys::SameLine(); }
  inline auto separator() -> void { sys::Separator(); }

  struct debug_field
  {
    template <typename... Args>
    inline debug_field(
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

} // namespace qdebugenv::imgui