#pragma once

#include <optional>
#include <qvariantmap.h>
#include <corona/modules/geoservice/detail/export.h>

namespace corona::modules::geoservice
{
  using namespace std::string_view_literals;

  /// \brief Name of the QML plugin.
  /// \details Used as prefix in QML plugin properties.
  constexpr inline auto plugin_name = "cgs"sv;

  /// \brief Small helper struct that provides functional of retrieving QML plugin parameters info.
  struct parameters
  {
    /// \brief List of parameters that can be used in the plugin properties in QML.
    enum parameter_t : char
    {
      /// \brief Directory where config files are located.
      target_config_directory, // NOLINT(*-identifier-naming)

      /// \brief Directory where cache files will be located.
      target_cache_directory // NOLINT(*-identifier-naming)
    };

    /// \brief Converts parameter enum to QML-represented name.
    /// \details Used in QML plugin properties.
    /// \param p Parameter enumeration.
    /// \return QML-represented name as <code>std::string</code>.
    [[nodiscard]] constexpr static auto parameter(parameter_t p) noexcept -> std::string {
      switch(p) {
        case target_config_directory: return fmt::format("{}.{}", plugin_name, "mapping.targetConfigDirectory");
        case target_cache_directory: return fmt::format("{}.{}", plugin_name, "mapping.targetCacheDirectory");
        default: return "";
      }
    }

    [[nodiscard]] static auto parse(::QVariantMap const& parameters, parameter_t p) -> option<::QString> {
      auto const search_for = parameters::parameter(p);
      if(parameters.contains(::QString::fromStdString(search_for)))
        return parameters.value(::QString::fromStdString(search_for)).toString();
      return none;
    }

    [[nodiscard]] static auto parse(::QVariantMap const& parameters, parameter_t p, std::function<void()> const& on_failure) -> option<::QString> {
      auto const search_for = parameters::parameter(p);
      if(parameters.contains(::QString::fromStdString(search_for)))
        return parameters.value(::QString::fromStdString(search_for)).toString();
      if(on_failure)
        on_failure();
      return none;
    }
  };
} // namespace corona::modules::geoservice