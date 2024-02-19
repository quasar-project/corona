#pragma once

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtGui/QColor>

namespace gui::theme
{
  class ThemeProvider;

  // ReSharper disable once CppClassCanBeFinal
  class ThemeQMLWrapperInternal : public QObject
  {
    Q_OBJECT

    public:
      explicit ThemeQMLWrapperInternal(ThemeProvider* ptr, QObject* parent = nullptr);

      Q_INVOKABLE [[nodiscard]] QColor color(const QString& name) const;

    private:
      ThemeProvider* m_ptr;
  };
}

Q_DECLARE_METATYPE(gui::theme::ThemeQMLWrapperInternal*)

namespace gui::theme
{
  // ReSharper disable once CppClassCanBeFinal
  class ThemeQMLWrapper : public QObject
  {
    public:
    enum class PaletteType
    {
      Light,
      Dark
    };
    Q_ENUM(PaletteType)

    private:
    Q_OBJECT
    Q_PROPERTY(PaletteType mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(gui::theme::ThemeQMLWrapperInternal* io READ io WRITE setIO NOTIFY ioChanged FINAL)

    public:
      explicit ThemeQMLWrapper(ThemeProvider* ptr, QObject* parent = nullptr);

      Q_INVOKABLE void toggle();

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