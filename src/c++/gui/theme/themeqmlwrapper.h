#pragma once

#include <afx>
#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtGui/QColor>

namespace gui::theme
{
  class ThemeProvider;

  // ReSharper disable once CppClassCanBeFinal
  class ThemeQMLWrapperInternal : public qt::Object
  {
    Q_OBJECT

    public:
      explicit ThemeQMLWrapperInternal(ThemeProvider* ptr, qt::Object* parent = nullptr);

      invokable [[nodiscard]] QColor color(const qt::String& name) const;

    private:
      ThemeProvider* m_ptr;
  };
}

qt_register_class_in_metasystem(gui::theme::ThemeQMLWrapperInternal*)

namespace gui::theme
{
  // ReSharper disable once CppClassCanBeFinal
  class ThemeQMLWrapper : public qt::Object
  {
    public:
    enum class PaletteType
    {
      Light,
      Dark
    };
    qt_registered_enum(PaletteType)

    private:
    Q_OBJECT
    qt_property(PaletteType mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    qt_property(gui::theme::ThemeQMLWrapperInternal* io READ io WRITE setIO NOTIFY ioChanged FINAL)

    public:
      explicit ThemeQMLWrapper(ThemeProvider* ptr, qt::Object* parent = nullptr);

      invokable void toggle();

      [[nodiscard]] PaletteType mode() const;
      void setMode(PaletteType mode);

      [[nodiscard]] ThemeQMLWrapperInternal* io() const;
      void setIO(ThemeQMLWrapperInternal* io);
      void updateIO();

      signals:
        void modeChanged();
        void ioChanged();

    private:
      static void emplace();

    private:
      ThemeProvider* m_ptr;
      ThemeQMLWrapperInternal* m_internal{};
  };
}