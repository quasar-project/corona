#pragma once

#include <leaf/global.h>
#include <QtCore/QVariantList>
#include <QtPositioning/QGeoCoordinate>

namespace map::utils
{
  using namespace leaf::types;

  class Orthodrom
  {
    public:
      Orthodrom();
      Orthodrom(const QGeoCoordinate& first, const QGeoCoordinate& second);

      [[nodiscard]] auto get() const noexcept -> QList<QVariant>;
      void set(const QGeoCoordinate& first, const QGeoCoordinate& second) noexcept;

      [[nodiscard]] auto latitude_at(f64 longitude) const noexcept -> f64;

    protected:
      void distribute() noexcept;
      [[nodiscard]] auto distance() const noexcept -> f64;

    private:
      f64 a{};
      f64 b{};
      QGeoCoordinate first;
      QGeoCoordinate second;
      QList<QVariant> path;
  };
}


