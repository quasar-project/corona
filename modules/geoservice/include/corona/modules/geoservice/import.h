#pragma once

#include <qglobal.h>
#include "detail/export.h"

namespace corona::modules::geoservice // NOLINT(*-concat-nested-namespaces)
{
  inline namespace imports
  {
    CORONA_BOOTSTRAP_GEOSERVICE_API [[maybe_unused]] [[nodiscard]] auto import_plugin() -> bool;
  } // namespace imports
} // namespace corona::modules::geoservice
