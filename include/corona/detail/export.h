#pragma once

#include <floppy/floppy.h>
#include <floppy/traits.h>

#if defined(FLOPPY_OS_WINDOWS)
# if defined(CORONA_LIBRARY)
#   define CORONA_API __declspec(dllexport)
# elif defined(CORONA_STATIC_LIBRARY)
#   define CORONA_API
# else
#   define CORONA_API __declspec(dllimport)
# endif
#else
# define CORONA_API
#endif

/// \brief Corona library namespace.
namespace corona
{
  using std::string;
  using std::string_view;
  using namespace fl::types;

  /// \brief Namespace for project metadata.
  namespace meta
  {
    /// \brief Meta information about the library.
    [[maybe_unused]] constexpr inline auto corona_meta = fl::meta::project_meta(
        fl::meta::version(
            CORONA_PROJECT_VERSION_MAJOR,
            CORONA_PROJECT_VERSION_MINOR,
            CORONA_PROJECT_VERSION_PATCH
        ),
        string_view(CORONA_TARGET_NAME),
        CORONA_ORGANIZATION_URL,
        CORONA_ORGANIZATION_NAME
    );
  } // namespace meta
} // namespace corona