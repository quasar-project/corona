#include <qqml.h>
#include <fmt/core.h>

namespace qml
{
  [[maybe_unused]] volatile auto register_misc_ = []() { // NOLINT(*-identifier-naming)
    fmt::println("registering io.corona.standalone.map qml types");
    ::qmlRegisterModule("io.corona.standalone.map", 1, 0);
    ::qmlRegisterType(QUrl("qrc:/qml/map/MapView.qml"), "io.corona.standalone.map", 1, 0, "MapView");
    ::qmlRegisterType(QUrl("qrc:/qml/map/MapModeButton.qml"), "io.corona.standalone.map.ui", 1, 0, "MapModeButton");
    ::qmlRegisterType(QUrl("qrc:/qml/ui/SimpleCheckableToolButton.qml"), "io.corona.standalone.ui", 1, 0, "SimpleCheckableToolButton");
    return true;
  }();
} // namespace qml