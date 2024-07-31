#include <corona-standalone/map/class_map_view_manager.hh>

#include <corona/bootstrap/geoservice/class_config.h>

namespace corona::standalone::map
{
  CMapViewManager::CMapViewManager(fl::filesystem::application_dirs const& dirs, ::QObject* parent)
    : ::QObject(parent)
    , state_config_(
        "map_state.conf.yml",
        dirs.config_dir() / "states",
        fl::saving_policy::autosave
      )
  {
    llog::debug("CMapViewManager: loading previous map state");
    this->state_config_.load();
    auto state_gadget = CMapStateGadget();
    state_gadget.setCoordinate(::QGeoCoordinate(this->state_config_().pan.latitude, this->state_config_().pan.longitude));
    state_gadget.setZoomLevel(this->state_config_().zoom);
    this->setState(state_gadget);

    llog::debug("CMapViewManager: initializing configuration file");
    auto const cache_root = dirs.cache_dir() / "geoservice" / "offline";
    if(not exists(cache_root)) {
      create_directories(cache_root);
      create_directories(cache_root / "street");
      create_directories(cache_root / "satellite");
      create_directories(cache_root / "hybrid");
    }
    auto const cfg_root = dirs.config_dir() / "geoservice";
    if(not exists(cfg_root))
      create_directories(cfg_root);
    auto const cfg_path = cfg_root / "geoservice.conf.yml";
    this->setConfigPath(::QString::fromStdString(cfg_root.generic_string()));
    this->setStoragePath(::QString::fromStdString(cache_root.generic_string()));
    llog::trace("CMapViewManager: config path: \'{}\'", cfg_root.generic_string());
    llog::trace("CMapViewManager: storage path: \'{}\'", cache_root.generic_string());
    if(exists(cfg_path)) {
      llog::trace("CMapViewManager: configuration file already exists, skipping creation");
      return;
    }
    llog::debug("CMapViewManager: emplacing configuration file");
    auto cfg = fl::configuration_file<fl::serialization::format::yaml, geoservice::ConfigData>(cfg_path, fl::saving_policy::explicit_);
    cfg.values_mut().views[0].offline_url = fmt::format("{}/street/{{z}}/{{x}}/{{y}}", cache_root.generic_string());
    cfg.values_mut().views[1].offline_url = fmt::format("{}/satellite/{{z}}/{{x}}/{{y}}", cache_root.generic_string());
    cfg.values_mut().views[2].offline_url = fmt::format("{}/hybrid/{{z}}/{{x}}/{{y}}", cache_root.generic_string());
    cfg.save();
  }

  auto CMapViewManager::setState(CMapStateGadget const& state) -> void {
    llog::trace("CMapViewManager: setting new map state: {{[{}, {}]; z={}}}",
      state.coordinate().latitude(),
      state.coordinate().longitude(),
      state.zoomLevel()
    );
    this->state_gadget_ = state;
    this->state_config_.values_mut().zoom = state.zoomLevel();
    this->state_config_.values_mut().pan.latitude = state.coordinate().latitude();
    this->state_config_.values_mut().pan.longitude = state.coordinate().longitude();
    llog::trace("CMapViewManager: saving new map state");
    emit this->stateChanged();
  }
} // namespace corona::standalone::map