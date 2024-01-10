#pragma once

#include <afx>

namespace map::utils
{
  [[nodiscard]] auto map_quick_item_zoomlevel(f64 lat, f32 meters_per_px) noexcept -> f64;
  [[nodiscard]] auto geocoordinate_to_webmercator(const qt::GeoCoordinate& coord, u8 zoom = 19) noexcept -> qt::PointF;
}