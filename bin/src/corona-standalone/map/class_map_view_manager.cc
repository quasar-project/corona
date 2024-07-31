#include <corona-standalone/map/class_map_view_manager.hh>

#include <corona/bootstrap/geoservice/class_config.h>

namespace corona::standalone::map
{
  CMapViewManager::CMapViewManager(fl::filesystem::application_dirs const& dirs, ::QObject* parent)
    : ::QObject(parent)
  {
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
} // namespace corona::standalone::map