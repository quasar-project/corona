#pragma once

#include <floppy/floppy.h>
#include <floppy/logging.h>
#include <corona/detail/export.h>

namespace corona
{
  namespace bootstrap {} // namespace bootstrap
  namespace standalone
  {
    using namespace ::floppy::types;
    using namespace ::corona::bootstrap;
    namespace llog = ::floppy::log;
  } // namespace standalone
} // namespace corona