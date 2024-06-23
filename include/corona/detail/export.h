#pragma once

#include <floppy/floppy.h>

#if defined(FLOPPY_OS_WINDOWS)
# if defined(CORONA_LIBRARY)
#   define CORONA_EXPORT __declspec(dllexport)
# elif defined(CORONA_STATIC_LIBRARY)
#   define CORONA_EXPORT
# else
#   define CORONA_EXPORT __declspec(dllimport)
# endif
#else
# define CORONA_EXPORT
#endif

namespace corona
{
  using std::string;
  using std::string_view;
  using namespace fl::types;

  namespace meta
  {
    [[maybe_unused]] constexpr inline auto floppy_meta = fl::meta::project_meta(
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