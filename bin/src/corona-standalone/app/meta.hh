#pragma once

#include <corona-standalone/definitions.hh>

namespace corona::standalone::app::meta
{
  /// \brief Meta information about the standalone app.
  [[maybe_unused]] constexpr inline auto corona_meta = fl::meta::project_meta(
    fl::meta::version(
      CORONA_STANDALONE_PROJECT_VERSION_MAJOR,
      CORONA_STANDALONE_PROJECT_VERSION_MINOR,
      CORONA_STANDALONE_PROJECT_VERSION_PATCH
    ),
    string_view(CORONA_STANDALONE_TARGET_NAME),
    CORONA_STANDALONE_ORGANIZATION_URL,
    CORONA_STANDALONE_ORGANIZATION_NAME
  );
} // namespace corona::standalone::app::meta