#pragma once

#include <floppy/floppy.h>
#include <floppy/traits.h>

#if defined(FLOPPY_OS_WINDOWS)
# if defined(CBI_LIBRARY)
#   define CORONA_BOOTSTRAP_IMRENDERER_API __declspec(dllexport)
# elif defined(CBI_STATIC_LIBRARY)
#   define CORONA_BOOTSTRAP_IMRENDERER_API
# else
#   define CORONA_BOOTSTRAP_IMRENDERER_API __declspec(dllimport)
# endif
#else
# define CORONA_BOOTSTRAP_IMRENDERER_API
#endif

namespace corona::bootstrap::imrenderer
{
  using namespace fl::types;
  using namespace fl::literals;

  /// \brief Namespace for project metadata.
  namespace meta
  {
    /// \brief Meta information about the library.
    [[maybe_unused]] constexpr inline auto meta_object = fl::meta::project_meta(
      fl::meta::version(
        #if defined(CBI_PROJECT_VERSION_MAJOR)
        CBI_PROJECT_VERSION_MAJOR,
        #else // defined(CBI_PROJECT_VERSION_MAJOR)
        0,
        #endif // defined(CBI_PROJECT_VERSION_MAJOR)
        #if defined(CBI_PROJECT_VERSION_MINOR)
        CBI_PROJECT_VERSION_MINOR,
        #else // defined(CBI_PROJECT_VERSION_MINOR)
        1,
        #endif // defined(CBI_PROJECT_VERSION_MINOR)
        #if defined(CBI_PROJECT_VERSION_PATCH)
        CBI_PROJECT_VERSION_PATCH
        #else // defined(CBI_PROJECT_VERSION_PATCH)
        0
        #endif // defined(CBI_PROJECT_VERSION_PATCH)
      ),
      std::string_view(
        #if defined(CBI_TARGET_NAME)
        CBI_TARGET_NAME
        #else // defined(CBI_TARGET_NAME)
        "corona::bootstrap-imrenderer"
        #endif // defined(QDE_TARGET_NAME)
      ),
      "io.github.whs31.qdebugenv",
      "whs31"
    );

    constexpr inline auto qml_namespace_rendering = "io.corona.rendering.immediate";
  } // namespace meta
} // namespace corona::bootstrap::imrenderer