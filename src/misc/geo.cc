#include <corona/misc/geo.h>

#include <qgeocoordinate.h>

namespace
{
  constexpr auto map_scale_ratio = 156'543.03392;
} // namespace

namespace corona::geo
{
  auto osm_object_zoom_level(f64 latitude, f32 ratio) noexcept -> f64 {
    using std::cos;
    using fl::math::is_null;
    using fl::math::log;
    using fl::math::to_radians;

    if(is_null(ratio))
      return log(2., ::map_scale_ratio * cos(to_radians(latitude)));
    return log(2., ::map_scale_ratio * cos(to_radians(latitude)) / ratio);
  }

  auto to_webmercator(QGeoCoordinate const& coordinate, u8 zoom) noexcept -> fl::math::point2d<f32> {
    using std::numbers::pi_v;
    using std::asinh;
    using std::tan;
    using fl::math::to_radians;

    return {
      static_cast<f32>((1.F - asinh(tan(to_radians(coordinate.latitude()))) / pi_v<f32>) / 2.0F * (1 << zoom)),
      static_cast<f32>((coordinate.longitude() + 180.F) / 360.F * (1 << zoom))
    };
  }
} // namespace corona::geo