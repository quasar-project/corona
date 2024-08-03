#pragma once

#include <qobject.h>
#include <qabstractitemmodel.h>
#include <corona/misc/geo.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::map
{
  class Ruler : public ::QAbstractListModel
  {
    Q_OBJECT
    Q_PROPERTY(qreal lastLatitude READ lastLatitude WRITE setLastLatitude NOTIFY lastLatitudeChanged FINAL)
    Q_PROPERTY(qreal lastLongitude READ lastLongitude WRITE setLastLongitude NOTIFY lastLongitudeChanged FINAL)
    Q_PROPERTY(qreal totalLength READ totalLength WRITE setTotalLength NOTIFY totalLengthChanged FINAL)

   public:
    enum Roles
    {
      Index = Qt::UserRole + 1,
      Segment,
      SegmentLength,
      SegmentCenter
    };

    explicit Ruler(::QObject* parent = nullptr);
    virtual ~Ruler() override;

    [[nodiscard]] virtual auto rowCount(::QModelIndex const& parent) const -> int override;
    [[nodiscard]] virtual auto data(::QModelIndex const& index, int role) const -> ::QVariant override;
    [[nodiscard]] virtual auto roleNames() const -> ::QHash<int, ::QByteArray> override;

    [[nodiscard]] auto lastLatitude() const -> qreal;
    [[nodiscard]] auto lastLongitude() const -> qreal;
    [[nodiscard]] auto totalLength() const -> qreal;

    auto setLastLatitude(qreal latitude) -> void;
    auto setLastLongitude(qreal longitude) -> void;
    auto setTotalLength(qreal length) -> void;

    Q_INVOKABLE void setRoute(QList<QGeoCoordinate> const& route);
    Q_INVOKABLE void resetRoute();
    Q_INVOKABLE void insertPoint(QGeoCoordinate const& point, qsizetype index);
    Q_INVOKABLE void removePoint(qsizetype index);
    Q_INVOKABLE void updatePoint(QGeoCoordinate const& point, qsizetype index);
    Q_INVOKABLE QGeoCoordinate center(qsizetype index) const;
    Q_INVOKABLE qreal angle(QGeoCoordinate const& first, QGeoCoordinate const& second) const;

   signals:
    void totalLengthChanged();
    void lastLatitudeChanged();
    void lastLongitudeChanged();

   private:
    QList<QVariantList> segments_;
    QList<QGeoCoordinate> path_;
    QList<QGeoCoordinate> segment_centers_;
    geo::COrthodrom orthodrom_;
    f64 last_latitude_;
    f64 last_longitude_;
    f64 total_length_;
  };
} // namespace corona::standalone::map