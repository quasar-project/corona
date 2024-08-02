#include <corona-standalone/gui/theme/qml/class_theme_wrapper.hh>

#include <corona-standalone/app/meta.hh>

namespace corona::standalone::gui::theme::qml
{
  CThemeWrapper::CThemeWrapper(::QObject* parent)
    : QObject(parent)
    , theme_(fl::make_box<CTheme>(fl::application_dirs(corona::standalone::app::meta::corona_meta)))
  {
    this->update();
  }

  auto CThemeWrapper::unwrap() -> CTheme& { return this->theme_.ref_mut(); }
  auto CThemeWrapper::palette() const -> Palette const& {
    return this->theme_.ref().palette();
  }

  auto CThemeWrapper::mode() const -> CThemeWrapper::Mode {
    return static_cast<Mode>(this->theme_.ref().mode());
  }

  auto CThemeWrapper::themeName() const -> ::QString {
    return QString::fromStdString(string(this->theme_.ref().theme_name()));
  }

  auto CThemeWrapper::setMode(CThemeWrapper::Mode mode) -> void {
    this->unwrap().set_mode(static_cast<theme::Mode>(mode));
    emit this->paletteChanged();
    emit this->modeChanged();
  }

  auto CThemeWrapper::setThemeName(::QString const& name) -> void {
    try {
      this->unwrap().set_theme_name(name.toStdString());
    } catch (std::exception const& e) {
      llog::error("failed to set theme name: {}", e.what());
      return;
    }
    emit this->themeNameChanged();
  }

  void CThemeWrapper::swapMode() {
    this->unwrap().swap_mode();
    this->update();
  }

  auto CThemeWrapper::update() -> void {
    emit this->paletteChanged();
    emit this->themeNameChanged();
    emit this->modeChanged();
  }
} // namespace corona::standalone::gui::theme::qml