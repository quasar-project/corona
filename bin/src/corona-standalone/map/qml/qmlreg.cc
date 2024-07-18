#include <qqml.h>
#include <fmt/core.h>

namespace qml
{
  [[maybe_unused]] volatile auto register_map_ = []() { // NOLINT(*-identifier-naming)
    fmt::println("registering io.corona.standalone.map qml types");
    qmlRegisterModule("io.corona.standalone.map", 1, 0);
    qmlRegisterType(QUrl("qrc:/qml/map/MapView.qml"), "io.corona.standalone.map", 1, 0, "MapView");
    return true;
  }();
} // namespace qml