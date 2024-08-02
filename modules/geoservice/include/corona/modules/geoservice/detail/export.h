#pragma once

#include <floppy/floppy.h>
#include <floppy/traits.h>

#if defined(FLOPPY_OS_WINDOWS)
# if defined(CBG_LIBRARY)
#   define CORONA_BOOTSTRAP_GEOSERVICE_API __declspec(dllexport)
# elif defined(CBG_STATIC_LIBRARY)
#   define CORONA_BOOTSTRAP_GEOSERVICE_API
# else
#   define CORONA_BOOTSTRAP_GEOSERVICE_API __declspec(dllimport)
# endif
#else
# define CORONA_BOOTSTRAP_GEOSERVICE_API
#endif

namespace corona::modules::geoservice
{
  using namespace fl::types;
  using namespace fl::literals;

  /// \brief Namespace for project metadata.
  namespace meta
  {
    /// \brief Meta information about the library.
    [[maybe_unused]] constexpr inline auto meta_object = fl::meta::project_meta(
      fl::meta::version(
        #if defined(CBG_PROJECT_VERSION_MAJOR)
        CBG_PROJECT_VERSION_MAJOR,
        #else // defined(CBG_PROJECT_VERSION_MAJOR)
        0,
        #endif // defined(CBG_PROJECT_VERSION_MAJOR)
        #if defined(CBG_PROJECT_VERSION_MINOR)
        CBG_PROJECT_VERSION_MINOR,
        #else // defined(CBG_PROJECT_VERSION_MINOR)
        1,
        #endif // defined(CBG_PROJECT_VERSION_MINOR)
        #if defined(CBG_PROJECT_VERSION_PATCH)
        CBG_PROJECT_VERSION_PATCH
        #else // defined(CBG_PROJECT_VERSION_PATCH)
        0
        #endif // defined(CBG_PROJECT_VERSION_PATCH)
      ),
      std::string_view(
        #if defined(CBG_TARGET_NAME)
        CBG_TARGET_NAME
        #else // defined(CBG_TARGET_NAME)
        "corona::bootstrap-geoservice"
        #endif // defined(CBG_TARGET_NAME)
      ),
      "io.github.maybe-unused.qnx",
      "maybe-unused"
    );
  } // namespace meta
} // namespace corona::modules::geoservice