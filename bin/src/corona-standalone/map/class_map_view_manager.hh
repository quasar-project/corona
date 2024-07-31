#pragma once

#include <qobject.h>
#include <floppy/directories.h>
#include <qgeocoordinate.h>
#include <corona-standalone/definitions.hh>
#include <corona-standalone/map/class_map_memento.hh>

namespace corona::standalone::map
{
  class CMapStateGadget
  {
    Q_GADGET
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate FINAL)
    Q_PROPERTY(qreal zoomLevel READ zoomLevel WRITE setZoomLevel FINAL)

   public:
    CMapStateGadget() = default;
    [[nodiscard]] auto coordinate() const -> ::QGeoCoordinate { return this->coordinate_; }
    auto setCoordinate(::QGeoCoordinate const& coordinate) -> void { this->coordinate_ = coordinate; }

    [[nodiscard]] auto zoomLevel() const -> f64 { return this->zoom_level_; }
    auto setZoomLevel(f64 zoom_level) -> void { this->zoom_level_ = zoom_level; }

   private:
    ::QGeoCoordinate coordinate_ {};
    f64 zoom_level_ {};
  };
} // namespace corona::standalone::map

Q_DECLARE_METATYPE(corona::standalone::map::CMapStateGadget)

namespace corona::standalone::map
{
  class CMapViewManager : public ::QObject // NOLINT(*-special-member-functions)
  {
   public:
    enum class MapMode : char
    {
      Street,
      Satellite,
      Hybrid,
      Auto
    };
    Q_ENUM(MapMode)

   private:
    Q_OBJECT
    Q_PROPERTY(QString configPath READ configPath WRITE setConfigPath NOTIFY configPathChanged FINAL)
    Q_PROPERTY(QString storagePath READ storagePath WRITE setStoragePath NOTIFY storagePathChanged FINAL)
    Q_PROPERTY(corona::standalone::map::CMapStateGadget state READ state WRITE setState NOTIFY stateChanged FINAL)
    Q_PROPERTY(MapMode mapMode READ mapMode WRITE setMapMode NOTIFY mapModeChanged FINAL)

   public:
    explicit CMapViewManager(fl::filesystem::application_dirs const& dirs, ::QObject* parent = nullptr);
    virtual ~CMapViewManager() override = default;

    [[nodiscard]] auto configPath() const -> ::QString { return this->config_path_; } // NOLINT(*-identifier-naming)
    auto setConfigPath(::QString const& path) -> void { // NOLINT(*-identifier-naming)
      this->config_path_ = path;
      emit this->configPathChanged();
    }

    [[nodiscard]] auto storagePath() const -> ::QString { return this->storage_path_; } // NOLINT(*-identifier-naming)
    auto setStoragePath(::QString const& path) -> void { // NOLINT(*-identifier-naming)
      this->storage_path_ = path;
      emit this->storagePathChanged();
    }

    [[nodiscard]] auto state() const -> CMapStateGadget { return this->state_gadget_; }
    auto setState(CMapStateGadget const& state) -> void;

    [[nodiscard]] auto mapMode() const -> MapMode { return this->map_mode_; }
    auto setMapMode(MapMode mode) -> void {
      this->map_mode_ = mode;
      emit this->mapModeChanged();
    }

   signals:
    void configPathChanged(); // NOLINT(*-identifier-naming)
    void storagePathChanged(); // NOLINT(*-identifier-naming)
    void stateChanged(); // NOLINT(*-identifier-naming)
    void mapModeChanged(); // NOLINT(*-identifier-naming)

   private:
    CMapStateMemento state_config_;
    CMapStateGadget state_gadget_;
    ::QString config_path_;
    ::QString storage_path_;
    MapMode map_mode_;
  };
} // namespace corona::standalone::map
