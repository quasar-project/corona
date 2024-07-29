#pragma once

#include <memory>
#include <qobject.h>
#include "src/corona-standalone/gui/theme/class_theme.hh"

namespace corona::standalone::gui::theme::qml
{
  class CThemeWrapper : public QObject
  {
   public:
    enum class Mode : char
    {
      Light,
      Dark
    };
    Q_ENUM(Mode)

   private:
    Q_OBJECT
    Q_PROPERTY(corona::standalone::gui::theme::Palette palette READ palette NOTIFY paletteChanged FINAL STORED true)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged FINAL STORED false)
    Q_PROPERTY(QString themeName READ themeName WRITE setThemeName NOTIFY themeNameChanged FINAL STORED false)

   public:
    explicit CThemeWrapper(::QObject* parent = nullptr);

    [[nodiscard]] auto unwrap() -> CTheme&;

    [[nodiscard]] auto palette() const -> corona::standalone::gui::theme::Palette const&;
    [[nodiscard]] auto mode() const -> Mode;
    [[nodiscard]] auto themeName() const -> ::QString; // NOLINT(*-identifier-naming)

    auto setMode(Mode mode) -> void; // NOLINT(*-identifier-naming)
    auto setThemeName(::QString const& name) -> void; // NOLINT(*-identifier-naming)

    Q_INVOKABLE void swapMode(); // NOLINT(*-identifier-naming)
    auto update() -> void;

   signals:
    void paletteChanged(); // NOLINT(*-identifier-naming)
    void modeChanged(); // NOLINT(*-identifier-naming)
    void themeNameChanged(); // NOLINT(*-identifier-naming)

   private:
    fl::box<CTheme> theme_;
  };
} // namespace corona::standalone::gui::theme::qml