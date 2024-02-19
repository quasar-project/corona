#pragma once

#include <leaf/global.h>

class QPointF;
class QGeoCoordinate;

namespace map::utils
{
  using namespace leaf::types;

  [[nodiscard]] auto map_quick_item_zoomlevel(f64 lat, f32 meters_per_px) noexcept -> f64;
  [[nodiscard]] auto geocoordinate_to_webmercator(const QGeoCoordinate& coord, u8 zoom = 19) noexcept -> QPointF;
}