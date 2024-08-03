#pragma once

#include <list>
#include <qgeocoordinate.h>
#include <floppy/euclid/point2d.h>
#include <corona/detail/export.h>

namespace corona::geo
{
  /// \brief Returns <i>QML MapQuickItem</i> zoom level for given latitude and pixel-to-meters ratio.
  /// \param latitude Object latitude in degrees.
  /// \param ratio Meters per pixel.
  /// \return Zoom level.
  CORONA_API [[nodiscard]] auto osm_object_zoom_level(f64 latitude, f32 ratio = 1) noexcept -> f64;

  CORONA_API [[nodiscard]] auto to_webmercator(::QGeoCoordinate const& coordinate, u8 zoom = 19) noexcept -> fl::math::point2d<f32>;

  class CORONA_API COrthodrom
  {
   public:
    using coordinate_type = ::QGeoCoordinate;

    COrthodrom();
    COrthodrom(coordinate_type const& from, coordinate_type const& to);

    [[nodiscard]] auto get() const noexcept -> std::list<coordinate_type> const&;
    auto set(coordinate_type const& from, coordinate_type const& to) -> void;

    [[nodiscard]] auto from() const noexcept -> coordinate_type const&;
    [[nodiscard]] auto to() const noexcept -> coordinate_type const&;
    [[nodiscard]] auto latitude_at(f64 longitude) const noexcept -> f64;

    [[nodiscard]] auto operator*() const noexcept -> decltype(this->get()) { return this->get(); }
    [[nodiscard]] auto operator[](f64 longitude) const noexcept -> f64 { return this->latitude_at(longitude); }

   private:
    [[nodiscard]] auto distance() const noexcept -> f64;
    auto distribute() noexcept -> void;

   private:
    std::list<coordinate_type> ft_;
    std::list<coordinate_type> path_;
    f64 a1_;
    f64 a2_;
  };
} // namespace corona::geo
