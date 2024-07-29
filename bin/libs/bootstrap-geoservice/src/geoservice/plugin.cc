#include <geoservice/plugin.hh>

#include <floppy/logging.h>
#include <corona/bootstrap/geoservice/detail/export.h>
#include <geoservice/class_mapping_manager_engine.hh>

namespace llog = floppy::log;

#if defined(CBG_STATIC_LIBRARY)
extern "C" CORONA_BOOTSTRAP_GEOSERVICE_API auto qt_plugin_query_metadata_v2() -> QT_PREPEND_NAMESPACE(QPluginMetaData);
extern "C" CORONA_BOOTSTRAP_GEOSERVICE_API auto qt_plugin_instance() -> QT_PREPEND_NAMESPACE(QObject)*;
[[maybe_unused]] auto qt_static_plugin_cgs_plugin() -> QT_PREPEND_NAMESPACE(QStaticPlugin) {
  auto const plugin = QT_PREPEND_NAMESPACE(QStaticPlugin) {
    qt_plugin_instance,
    qt_plugin_query_metadata_v2
  };
  return plugin;
}
#endif // defined(CBG_STATIC_LIBRARY)

QT_BEGIN_NAMESPACE

auto QGeoServicePluginCGS::createMappingManagerEngine(
  QVariantMap const& parameters,
  QGeoServiceProvider::Error* error,
  QString* errorString
) const -> QGeoMappingManagerEngine* {
  llog::trace("geoservice::QGeoServicePluginCGS: creating mapping manager engine");
  return new corona::bootstrap::geoservice::CGeoTiledMappingManagerEngineMap(parameters, error, errorString); // NOLINT(*-owning-memory)
}

QT_END_NAMESPACE