#include <corona-standalone/app/app.hh>

#include <corona/modules/qmlbind/qmlbind.h>
#include <corona-standalone/app/app_p.hh>
#include <corona-standalone/gui/theme/class_reveal.hh>
#include <corona-standalone/gui/theme/qml/class_theme_wrapper.hh>

namespace corona::standalone::app
{
  auto Corona::register_qml_types() -> void {
    namespace q = modules::qmlbind;

    llog::debug("Corona: registering qml types");

    q::module("io.corona.standalone.app")
      .singleton(this->impl_->app_dirs.ptr_mut(), "Directories")
      .singleton(this->impl_->theme.ptr_mut(), "Theme");

    q::module("io.corona.standalone.layouts")
      .qml_file("qrc:/qml/layouts/ApplicationMenuBar.qml");

    q::module("io.corona.standalone.map")
      .singleton(this->impl_->map_view_manager.ptr_mut(), "MapManager")
      .qml_file("qrc:/qml/map/MapView.qml")
      .qml_file("qrc:/qml/map/CoordinateTooltip.qml");

    q::module("io.corona.standalone.map.ui")
      .qml_file("qrc:/qml/map/MapModeButton.qml")
      .qml_file("qrc:/qml/map/MapStateMachine.qml", "StateMachine")
      .qml_file("qrc:/qml/map/MapToolbarButton.qml", "ToolbarButton")
      .qml_file("qrc:/qml/map/MapToolbar.qml", "Toolbar");

    q::module("io.corona.standalone.ui")
      .qml_file("qrc:/qml/ui/SimpleCheckableToolButton.qml");

    q::module("io.corona.standalone.theme")
      .component<gui::theme::qml::CThemeWrapper>()
      .component<gui::theme::CCircularReveal>()
      .qml_file("qrc:/qml/theme/CircularChanger.qml");
  }
} // namespace corona::standalone::app

