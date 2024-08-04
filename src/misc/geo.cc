#include <corona/misc/geo.h>

namespace
{
  using namespace corona;

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
      return log(2., ::map_scale_ratio * cos(to_radians(latitude))); // NOLINT(*-use-std-numbers) <false-positive>
    return log(2., ::map_scale_ratio * cos(to_radians(latitude)) / ratio); // NOLINT(*-use-std-numbers) <false-positive>
  }
} // namespace corona::geo