#include <qqml.h>
#include <fmt/core.h>

namespace qml
{
  [[maybe_unused]] volatile auto register_map_ = []() { // NOLINT(*-identifier-naming)
    fmt::println("registering io.corona.standalone.layouts qml types");
    qmlRegisterModule("io.corona.standalone.layouts", 1, 0);
    qmlRegisterType(QUrl("qrc:/qml/layouts/ApplicationMenuBar.qml"), "io.corona.standalone.layouts", 1, 0, "ApplicationMenuBar");
    return true;
  }();
} // namespace qml