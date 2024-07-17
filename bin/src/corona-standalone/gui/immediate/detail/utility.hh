#pragma once

#include <corona-standalone/definitions.hh>

namespace corona::standalone::gui::immediate::detail
{
  [[nodiscard]]
  [[gnu::always_inline]]
  inline auto make_null_terminated(char const* ptr, size_t size) -> string {
    auto t = string(ptr, size);
    if(t.back() == '\0')
      return t;
    return t + '\0';
  }

  [[nodiscard]]
  [[gnu::always_inline]]
  inline auto make_null_terminated(char const* begin, char const* end) -> string {
    return make_null_terminated(begin, end - begin);
  }
} // namespace corona::standalone::gui::immediate::detail
