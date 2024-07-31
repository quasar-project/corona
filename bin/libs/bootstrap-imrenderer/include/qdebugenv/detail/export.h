#pragma once

#include <floppy/floppy.h>
#include <floppy/traits.h>

#if defined(FLOPPY_OS_WINDOWS)
# if defined(QDE_LIBRARY)
#   define QDE_API __declspec(dllexport)
# elif defined(QDE_STATIC_LIBRARY)
#   define QDE_API
# else
#   define QDE_API __declspec(dllimport)
# endif
#else
# define QDE_API __attribute__((visibility("default")))
#endif

/// \brief Qt Quick Debug Environment library namespace.
namespace qdebugenv
{
  using namespace fl::types;
  using namespace fl::literals;

  /// \brief Namespace for project metadata.
  namespace meta
  {
    /// \brief Meta information about the library.
    [[maybe_unused]] constexpr inline auto qde_meta = fl::meta::project_meta(
      fl::meta::version(
        #if defined(QDE_PROJECT_VERSION_MAJOR)
        QDE_PROJECT_VERSION_MAJOR,
        #else // defined(QDE_PROJECT_VERSION_MAJOR)
        0,
        #endif // defined(QDE_PROJECT_VERSION_MAJOR)
        #if defined(QDE_PROJECT_VERSION_MINOR)
        QDE_PROJECT_VERSION_MINOR,
        #else // defined(QDE_PROJECT_VERSION_MINOR)
        1,
        #endif // defined(QDE_PROJECT_VERSION_MINOR)
        #if defined(QDE_PROJECT_VERSION_PATCH)
        QDE_PROJECT_VERSION_PATCH
        #else // defined(QDE_PROJECT_VERSION_PATCH)
        0
        #endif // defined(QDE_PROJECT_VERSION_PATCH)
      ),
      std::string_view(
        #if defined(QDE_TARGET_NAME)
        QDE_TARGET_NAME
        #else // defined(QDE_TARGET_NAME)
        "qdebugenv"
        #endif // defined(QDE_TARGET_NAME)
      ),
      "io.github.whs31.qdebugenv",
      "whs31"
    );

    constexpr inline auto qml_namespace_rendering = "io.qdebugenv.rendering";
  } // namespace meta
} // namespace qdebugenv