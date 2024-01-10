//
// Created by whs31 on 10.01.2024.
//

#include "orthodrom.h"

using namespace std;

constexpr f64 TO_RADIANS = numbers::pi / 180.0;
constexpr f64 TO_DEGREES = 180.0 / numbers::pi;
constexpr u32 SPACING = 10;
constexpr f64 SEA_MILE_IN_KM = 1.853;

auto to_radians(const f64 value) -> f64 { return value * TO_RADIANS; }
auto to_degrees(const f64 value) -> f64 { return value * TO_DEGREES; }

namespace map::utils
{
  Orthodrom::Orthodrom()
    : first({0.0, 0.0})
    , second({0.0, 0.0})
  {}

  Orthodrom::Orthodrom(const qt::GeoCoordinate& first, const qt::GeoCoordinate& second)
  {
    this->set(first, second);
  }

  auto Orthodrom::get() const noexcept -> qt::List<qt::Variant>
  {
    if(sin(first.longitude() - second.longitude()))
      return path;
    return { qt::Variant::fromValue(first), qt::Variant::fromValue(second) };
  }

  void Orthodrom::set(const qt::GeoCoordinate& first, const qt::GeoCoordinate& second) noexcept
  {
    this->first = first;
    this->second = second;
    a = tan(to_radians(first.latitude())) / sin(to_radians(second.longitude() - first.longitude()));
    b = tan(to_radians(second.latitude())) / sin(to_radians(second.longitude() - first.longitude()));
    this->distribute();
  }

  auto Orthodrom::latitude_at(f64 longitude) const noexcept -> f64
  {
    const auto angle = atan(a * sin(to_radians(longitude - first.longitude()))
      + b * sin(to_radians(second.longitude() - longitude)));
    return to_degrees(angle);
  }

  void Orthodrom::distribute() noexcept
  {
    ensure(not isnan(a), "nan in orthodrom (a)");
    ensure(not isnan(b), "nan in orthodrom (b)");

    path.clear();
    path << qt::Variant::fromValue(second);
    const auto az = first.azimuthTo(second);
    auto d = this->distance();
    while(d - SPACING > 0)
    {
      const auto tmp = first.atDistanceAndAzimuth(d * 1000, az);
      path << qt::Variant::fromValue(qt::GeoCoordinate(this->latitude_at(tmp.longitude()), tmp.longitude(), 0));
      d -= SPACING;
    }

    if(const auto tmp = first.atDistanceAndAzimuth(d, az); tmp.longitude() - second.longitude() != 0)
      path << qt::Variant::fromValue(qt::GeoCoordinate(this->latitude_at(tmp.longitude()), tmp.longitude(), 0));
    else
      path << qt::Variant::fromValue(tmp);
  }

  auto Orthodrom::distance() const noexcept -> f64
  {
    return to_degrees(
      acos(
        sin(
          to_radians(first.latitude())
        ) * sin(
          to_radians(second.latitude())
        ) + cos(
          to_radians(first.latitude())
        ) * cos(
          to_radians(second.latitude())
        )* cos(
          to_radians(second.longitude() - first.longitude())
        )
      )
    ) * 60.0 * SEA_MILE_IN_KM;
  }
}
