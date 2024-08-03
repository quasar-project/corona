#pragma once

#include <floppy/euclid/point2d.h>
#include <corona/detail/export.h>

class QGeoCoordinate;

namespace corona::geo
{
  /// \brief Returns <i>QML MapQuickItem</i> zoom level for given latitude and pixel-to-meters ratio.
  /// \param latitude Object latitude in degrees.
  /// \param ratio Meters per pixel.
  /// \return Zoom level. 
  CORONA_API [[nodiscard]] auto osm_object_zoom_level(f64 latitude, f32 ratio = 1) noexcept -> f64;

  CORONA_API [[nodiscard]] auto to_webmercator(::QGeoCoordinate const& coordinate, u8 zoom = 19) noexcept -> fl::math::point2d<f32>;
} // namespace corona::geo
