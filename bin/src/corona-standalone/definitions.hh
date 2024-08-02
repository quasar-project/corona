#pragma once

#include <floppy/floppy.h>
#include <floppy/logging.h>
#include <corona/detail/export.h>

namespace corona
{
  namespace modules {} // namespace modules
  namespace standalone
  {
    using namespace ::fl::types;
    using namespace modules;
    namespace llog = ::floppy::log;
  } // namespace standalone
} // namespace corona