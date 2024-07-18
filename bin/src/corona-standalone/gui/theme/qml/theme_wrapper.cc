#include <corona-standalone/gui/theme/qml/theme_wrapper.hh>

#include <qqml.h>
#include <corona-standalone/app/meta.hh>
#include <corona-standalone/gui/theme/reveal.hh>

namespace qml
{
  [[maybe_unused]] volatile auto register_ = []() { // NOLINT(*-identifier-naming)
    fmt::println("registering io.corona.standalone.theme qml types");
    qmlRegisterModule("io.corona.standalone.theme", 1, 0);
    qmlRegisterType<corona::standalone::gui::theme::qml::ThemeWrapper>("io.corona.standalone.theme", 1, 0, "ThemeWrapper");
    qmlRegisterType<corona::standalone::gui::theme::CircularReveal>("io.corona.standalone.theme", 1, 0, "CircularReveal");
    qmlRegisterType(QUrl("qrc:/qml/theme/CircularChanger.qml"), "io.corona.standalone.theme", 1, 0, "CircularChanger");
    return true;
  }();
} // namespace qml

namespace corona::standalone::gui::theme::qml
{
  ThemeWrapper::ThemeWrapper(::QObject* parent)
    : QObject(parent)
    , theme_(fl::make_box<Theme>(fl::filesystem::application_dirs(corona::standalone::app::meta::corona_meta)))
  {
    this->update();
  }

  auto ThemeWrapper::unwrap() -> Theme& { return this->theme_.ref_mut(); }
  auto ThemeWrapper::palette() const -> Palette const& {
    return this->theme_.ref().palette();
  }

  auto ThemeWrapper::mode() const -> ThemeWrapper::Mode {
    return static_cast<Mode>(this->theme_.ref().mode());
  }

  auto ThemeWrapper::themeName() const -> ::QString {
    return QString::fromStdString(string(this->theme_.ref().theme_name()));
  }

  auto ThemeWrapper::setMode(ThemeWrapper::Mode mode) -> void {
    this->unwrap().set_mode(static_cast<theme::Mode>(mode));
    emit this->paletteChanged();
    emit this->modeChanged();
  }

  auto ThemeWrapper::setThemeName(::QString const& name) -> void {
    try {
      this->unwrap().set_theme_name(name.toStdString());
    } catch (std::exception const& e) {
      llog::error("failed to set theme name: {}", e.what());
      return;
    }
    emit this->themeNameChanged();
  }

  void ThemeWrapper::swapMode() {
    this->unwrap().swap_mode();
    this->update();
  }

  auto ThemeWrapper::update() -> void {
    emit this->paletteChanged();
    emit this->themeNameChanged();
    emit this->modeChanged();
  }
} // namespace corona::standalone::gui::theme::qml