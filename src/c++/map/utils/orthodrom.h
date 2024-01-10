#pragma once

#include <afx>
#include <QtCore/QVariantList>
#include <QtPositioning/QGeoCoordinate>

namespace map::utils
{
  class Orthodrom
  {
    public:
      Orthodrom();
      Orthodrom(const qt::GeoCoordinate& first, const qt::GeoCoordinate& second);

      [[nodiscard]] auto get() const noexcept -> qt::List<qt::Variant>;
      void set(const qt::GeoCoordinate& first, const qt::GeoCoordinate& second) noexcept;

      [[nodiscard]] auto latitude_at(f64 longitude) const noexcept -> f64;

    protected:
      void distribute() noexcept;
      [[nodiscard]] auto distance() const noexcept -> f64;

    private:
      f64 a{};
      f64 b{};
      qt::GeoCoordinate first;
      qt::GeoCoordinate second;
      qt::List<qt::Variant> path;
  };
}


