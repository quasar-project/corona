#pragma once

#include <qstring.h>
#include <qurl.h>
#include "detail/export.h"

namespace corona::modules::qmlbind // NOLINT(*-concat-nested-namespaces)
{
  inline namespace literals
  {
    [[maybe_unused]] inline auto operator""_qs(char16_t const* str, usize size) noexcept -> ::QString {
      return ::QString(::QStringPrivate(nullptr, const_cast<char16_t*>(str), static_cast<qsizetype>(size))); // NOLINT(*-pro-type-const-cast)
    }

    [[maybe_unused]] inline auto operator""_qurl(char16_t const* str, usize size) noexcept -> ::QUrl {
      return { operator""_qs(str, size) };
    }
  } // namespace literals
} // namespace corona::modules::qmlbind