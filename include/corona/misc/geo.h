#pragma once

#include <corona/detail/export.h>

namespace corona::geo
{
  /// \brief Returns <i>QML MapQuickItem</i> zoom level for given latitude and pixel-to-meters ratio.
  /// \param latitude Object latitude in degrees.
  /// \param ratio Meters per pixel.
  /// \return Zoom level.
  CORONA_API [[nodiscard]] auto osm_object_zoom_level(f64 latitude, f32 ratio = 1) noexcept -> f64;
} // namespace corona::geo
