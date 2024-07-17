#include <corona-standalone/gui/immediate/generic.hh>

#include <qqml.h>
#include <fmt/core.h>

namespace qml
{
  [[maybe_unused]] volatile auto register_imgui_ = []() { // NOLINT(*-identifier-naming)
    fmt::println("registering io.corona.standalone.gui qml types");
    qmlRegisterModule("io.corona.standalone.imgui", 1, 0);
    qmlRegisterType<corona::standalone::gui::immediate::GenericItem>("io.corona.standalone.imgui", 1, 0, "ImmediateGuiGenericRenderer");
    qmlRegisterType(QUrl("qrc:/qml/immediate/Renderer.qml"), "io.corona.standalone.imgui", 1, 0, "ImmediateGuiMainRenderer");
    return true;
  }();
} // namespace qml

namespace corona::standalone::gui::immediate
{
  GenericItem::GenericItem(QRhiImguiItem* parent)
    : QRhiImguiItem(parent)
  {}

  GenericItem::~GenericItem() = default;

  auto GenericItem::frame() -> void {
    for(auto& callback : this->callbacks_)
      callback();

    for(auto& drawable : this->drawables_)
      drawable->draw_debug_frame();
  }
} // namespace corona::standalone::gui::immediate