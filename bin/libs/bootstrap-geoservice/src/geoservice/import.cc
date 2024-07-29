#include <corona/bootstrap/geoservice/import.h>

#include <qplugin.h>
#include <qgeoserviceprovider.h>
#include <floppy/logging.h>
#include <corona/bootstrap/formatters.h>

namespace llog = floppy::log;

inline auto init_plugin() -> void {
  #if defined(CBG_STATIC_LIBRARY)
  Q_IMPORT_PLUGIN(cgs_plugin);
  #endif // defined(QNX_STATIC_LIBRARY)
}

namespace corona::bootstrap::geoservice // NOLINT(*-concat-nested-namespaces)
{
  inline namespace imports
  {
    [[maybe_unused]] [[nodiscard]] auto import_plugin() -> bool {
      ::init_plugin();
      llog::trace("geoservice: importing cgs geoservice plugin");
      llog::trace("geoservice: available service providers(): {}",
        ::QGeoServiceProvider::availableServiceProviders().join(", ")
      );
      if(not ::QGeoServiceProvider::availableServiceProviders().contains("cgs", Qt::CaseInsensitive)) {
        llog::error("geoservice: `cgs` not found in any known library paths");
        return false;
      }
      if(auto const t = ::QGeoServiceProvider("cgs"); t.error()) {
        llog::error("geoservice: `cgs` returned an error ({})", t.errorString());
        return false;
      }
      llog::debug("geoservice: `cgs` plugin imported successfully (geoservice/cgs)");
      return true;
    }
  } // namespace imports
} // namespace corona::bootstrap::geoservice