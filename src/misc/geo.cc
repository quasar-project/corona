#include <corona/misc/geo.h>

namespace
{
  using namespace corona;

  constexpr auto map_scale_ratio = 156'543.03392;
  constexpr auto orthodrom_spacing = 10;
  constexpr auto sea_mile_to_km = 1.853;

  [[nodiscard]] constexpr auto orthodrom_a1(::QGeoCoordinate const& first, ::QGeoCoordinate const& second) noexcept -> f64 {
    return std::tan(fl::math::to_radians(first.latitude())) / std::sin(fl::math::to_radians(second.longitude() - first.longitude()));
  }

  [[nodiscard]] constexpr auto orthodrom_a2(::QGeoCoordinate const& first, ::QGeoCoordinate const& second) noexcept -> f64 {
    return std::tan(fl::math::to_radians(second.latitude())) / std::sin(fl::math::to_radians(second.longitude() - first.longitude()));
  }
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

  COrthodrom::COrthodrom()
    : ft_{
        COrthodrom::coordinate_type(.0, .0),
        COrthodrom::coordinate_type(.0, .0)
      }
    , a1_(.0)
    , a2_(.0)
  {}

  COrthodrom::COrthodrom(COrthodrom::coordinate_type const& from, COrthodrom::coordinate_type const& to)
    : ft_{
        from,
        to
      }
    , a1_(::orthodrom_a1(from, to))
    , a2_(::orthodrom_a2(from, to))
  {}

  auto COrthodrom::get() const noexcept -> std::list<coordinate_type> const& {
    if(std::sin(this->from().longitude() - this->to().longitude()))
      return this->path_;
    return this->ft_;
  }

  auto COrthodrom::set(COrthodrom::coordinate_type const& from, COrthodrom::coordinate_type const& to) -> void {
    this->ft_ = { from, to };
    this->a1_ = ::orthodrom_a1(from, to);
    this->a2_ = ::orthodrom_a2(from, to);
    this->distribute();
  }

  auto COrthodrom::from() const noexcept -> COrthodrom::coordinate_type const& {
    return this->ft_.front();
  }

  auto COrthodrom::to() const noexcept -> COrthodrom::coordinate_type const& {
    return this->ft_.back();
  }

  auto COrthodrom::latitude_at(f64 longitude) const noexcept -> f64 {
    using fl::math::to_radians;
    using fl::math::to_degrees;
    return to_degrees(
      std::atan(this->a2_ * std::sin(to_radians(longitude - this->from().longitude()))
        + this->a1_ * std::sin(to_radians(this->to().longitude() - longitude))
    ));
  }

  auto COrthodrom::distance() const noexcept -> f64 {
    using fl::math::to_radians;
    using fl::math::to_degrees;

    auto const x = std::sin(to_radians(this->from().latitude())) * std::sin(to_radians(this->to().latitude()))
      + std::cos(to_radians(this->from().latitude())) * std::cos(to_radians(this->to().latitude()))
      * std::cos(to_radians(this->to().longitude() - this->from().longitude()));
    return to_degrees(std::acos(x)) * 60. * ::sea_mile_to_km;
  }

  auto COrthodrom::distribute() noexcept -> void {
    if(std::isnan(this->a1_) or std::isnan(this->a2_))
      return;
    this->path_.clear();
    this->path_.push_back(this->to());
    auto const az = this->from().azimuthTo(this->to());
    auto d = this->distance();
    while(d - ::orthodrom_spacing > 0) {
      auto const next = this->from().atDistanceAndAzimuth(d * 1000., az);
      this->path_.emplace_back((*this)[next.longitude()], next.longitude(), .0);
      d -= ::orthodrom_spacing;
    }

    auto const last = this->from().atDistanceAndAzimuth(d, az);
    if(last.longitude() - this->to().longitude() != 0.)
      this->path_.emplace_back((*this)[last.longitude()], last.longitude(), .0);
    else
      this->path_.push_back(last);
  }
} // namespace corona::geo