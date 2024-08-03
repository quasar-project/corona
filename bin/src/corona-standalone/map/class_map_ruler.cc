#include <corona-standalone/map/class_map_ruler.hh>

namespace
{
  [[nodiscard]] auto to_qvariantlist(std::list<::QGeoCoordinate> const& list) -> ::QVariantList {
    auto result = ::QVariantList();
    for(auto const& item : list)
      result.append(::QVariant::fromValue(item));
    return result;
  }
} // namespace

namespace corona::standalone::map
{
  Ruler::Ruler(::QObject* parent)
    : ::QAbstractListModel(parent)
  {}

  Ruler::~Ruler() = default;

  auto Ruler::rowCount(QModelIndex const& parent) const -> int { return this->segments_.size(); }
  auto Ruler::data(QModelIndex const& index, int role) const -> ::QVariant {
    if(not index.isValid())
      return {};
    switch(role) {
      case Index: return index.row();
      case Segment: return this->segments_[index.row()];
      case SegmentLength: return std::round(this->path_[index.row()].distanceTo(this->path_[index.row() + 1]));
      case SegmentCenter: return ::QVariant::fromValue(this->segment_centers_[index.row()]);
      default: return {};
    }
  }

  auto Ruler::roleNames() const -> ::QHash<int, ::QByteArray> {
    auto roles = ::QAbstractListModel::roleNames();
    roles[Index] = "index";
    roles[Segment] = "segment";
    roles[SegmentLength] = "segmentLength";
    roles[SegmentCenter] = "segmentCenter";
    return roles;
  }

  auto Ruler::lastLatitude() const -> qreal { return this->last_latitude_; }
  auto Ruler::lastLongitude() const -> qreal { return this->last_longitude_; }
  auto Ruler::totalLength() const -> qreal { return this->total_length_; }
  auto Ruler::setLastLatitude(qreal latitude) -> void {
    this->last_latitude_ = latitude;
    emit this->lastLatitudeChanged();
  }
  auto Ruler::setLastLongitude(qreal longitude) -> void {
    this->last_longitude_ = longitude;
    emit this->lastLongitudeChanged();
  }
  auto Ruler::setTotalLength(qreal length) -> void {
    this->total_length_ = length;
    emit this->totalLengthChanged();
  }

  void Ruler::setRoute(QList<QGeoCoordinate> const& route) {
    this->beginResetModel();
    this->segments_.clear();
    this->path_ = route;
    for(int i = 0; i < route.count() - 1; ++i) {
      this->orthodrom_ = geo::COrthodrom(route[i], route[i + 1]);
      auto const lst = this->orthodrom_.get();
      auto const qlist = ::QList<::QGeoCoordinate>(lst.begin(), lst.end());
      this->segments_.append(::to_qvariantlist(this->orthodrom_.get()));
      this->segment_centers_.append(this->center(this->segments_.size() - 1));
    }
    this->endResetModel();
    for(int i = 0; i < this->path_.size(); ++i)
      this->updatePoint(this->path_.at(i), i);
  }

  void Ruler::resetRoute() {
    this->setLastLatitude(0.);
    this->setLastLongitude(0.);
    this->beginRemoveRows(QModelIndex(), 0, this->segments_.size() - 1);
    this->segments_.clear();
    this->path_.clear();
    this->segment_centers_.clear();
    this->endRemoveRows();
  }

  void Ruler::insertPoint(QGeoCoordinate const& point, qsizetype index) {
    this->setLastLatitude(point.latitude());
    this->setLastLongitude(point.longitude());
    auto const points_count = this->segments_.size() + 1;
    if(index > this->path_.size() or index < 0)
      return;
    this->path_.insert(index, point);
    if(this->path_.size() == 1)
      return;
    if(this->path_.size() == 2) {
      this->beginInsertRows(QModelIndex(), 0, 0);
      this->orthodrom_ = geo::COrthodrom(this->path_[0], this->path_[1]);
      this->segment_centers_.insert(0, this->center(0));
      this->segments_.insert(0, ::to_qvariantlist(this->orthodrom_.get()));
      this->endInsertRows();
      return;
    }
    if(index == 0) {
      this->beginInsertRows(QModelIndex(), 0, 0);
      this->orthodrom_ = geo::COrthodrom(point, this->path_[0]);
      this->segment_centers_.insert(0, this->center(0));
      this->segments_.insert(0, ::to_qvariantlist(this->orthodrom_.get()));
      this->endInsertRows();
    } else if(index == this->path_.size() - 1) {
      this->beginInsertRows(QModelIndex(), this->segments_.size(), this->segments_.size());
      this->orthodrom_ = geo::COrthodrom(this->path_[points_count - 1], point);
      this->segment_centers_.insert(this->segments_.size(), this->center(this->segments_.size()));
      this->segments_.insert(this->segments_.size(), ::to_qvariantlist(this->orthodrom_.get()));
      this->endInsertRows();
    } else {
      this->orthodrom_ = geo::COrthodrom(this->path_[index - 1], this->path_[index]);
      this->segments_[index - 1] = ::to_qvariantlist(this->orthodrom_.get());
      this->segment_centers_[index - 1] = this->center(index - 1);
      emit this->dataChanged(this->createIndex(index - 1, 0), this->createIndex(index - 1, 0));

      this->beginInsertRows(QModelIndex(), index, index);
      this->orthodrom_ = geo::COrthodrom(this->path_[index], this->path_[index + 1]);
      this->segment_centers_.insert(index, this->center(index));
      this->segments_.insert(index, ::to_qvariantlist(this->orthodrom_.get()));
      this->endInsertRows();
    }
  }
  void Ruler::removePoint(qsizetype index) {
    // todo https://github.com/whs31/quasar-software/blob/qt5/src/c%2B%2B/map/ruler.c%2B%2B
  }
  void Ruler::updatePoint(QGeoCoordinate const& point, qsizetype index) {}
  QGeoCoordinate Ruler::center(qsizetype index) const { return QGeoCoordinate(); }
  qreal Ruler::angle(QGeoCoordinate const& first, QGeoCoordinate const& second) const { return 0; }
} // namespace corona::standalone::map