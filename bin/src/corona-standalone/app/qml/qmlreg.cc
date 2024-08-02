#include <qqml.h>
#include <fmt/core.h>
#include <corona/modules/qmlbind/qmlbind.h>

namespace qml
{
  [[maybe_unused]] volatile auto register_misc_ = []() { // NOLINT(*-identifier-naming)
    namespace q = ::corona::modules::qmlbind;
    q::module("io.corona.standalone.map")
      .qml_file("qrc:/qml/map/MapView.qml");

    q::module("io.corona.standalone.map.ui")
      .qml_file("qrc:/qml/map/MapModeButton.qml")
      .qml_file("qrc:/qml/map/MapStateMachine.qml", "StateMachine")
      .qml_file("qrc:/qml/map/MapToolbarButton.qml", "ToolbarButton")
      .qml_file("qrc:/qml/map/MapToolbar.qml", "Toolbar");

    q::module("io.corona.standalone.ui")
      .qml_file("qrc:/qml/ui/SimpleCheckableToolButton.qml");
    return true;
  }();
} // namespace qml