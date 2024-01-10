//
// Created by whs31 on 10.01.2024.
//

#include "geomath.h"
#include <QtCore/QPointF>
#include <QtPositioning/QGeoCoordinate>

constexpr f64 MAP_SCALE_RATIO = 156'543.03392;

template<typename T>
concept Number = std::integral<T> or std::floating_point<T>;

template<Number T>
auto log_ratio(T base, T value) -> T { return std::log(value) / std::log(base); }

auto map::utils::map_quick_item_zoomlevel(const f64 lat, f32 meters_per_px) noexcept -> f64
{
  using namespace std;

  if(meters_per_px <= 0.0f)
    meters_per_px = 1.0f;
  return ::log_ratio(2.0, MAP_SCALE_RATIO * cos(lat * numbers::pi / 180.0) / meters_per_px);
}

auto map::utils::geocoordinate_to_webmercator(const qt::GeoCoordinate& coord, const u8 zoom) noexcept -> qt::PointF
{
  using namespace std;
  return {
    (1.0 - asinh(tan(coord.latitude() * numbers::pi / 180.0)) / numbers::pi) / 2.0 * (1 << zoom),
    (coord.longitude() + 180.0) / 360.0 * (1 << zoom)
  };
}
