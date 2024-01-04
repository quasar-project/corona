//
// Created by whs31 on 28.10.2023.
//

// todo rewrite fully
#pragma once

#include <map>
#include <memory>
#include <QtCore/QObject>
#include <QtGui/QColor>
#include <QtExtensions/QtExtensions>
#include <QtExtensions/QMLRegistration>
#include <QtExtensions/StaticConfig>

using std::map;
using std::unique_ptr;

namespace QtEx
{
  class ThemeImpl : public Qt::Object
  {
    Q_OBJECT

    constexpr static const char* FALLBACK = ":/qtx/themes/catpuccin.json";

    public:
      enum ThemePalette
      {
        Rosewater,
        Flamingo,
        Pink,
        Mauve,
        Red,
        Maroon,
        Peach,
        Yellow,
        Green,
        Teal,
        Sky,
        Sapphire,
        Blue,
        Lavender,
        Text,
        Subtext1,
        Subtext0,
        Overlay2,
        Overlay1,
        Overlay0,
        Surface2,
        Surface1,
        Surface0,
        Base,
        Mantle,
        Crust
      };

      enum ThemeMode
      {
        Dark,
        Light
      };

      enum PrimaryPalette
      {
        PrimaryDark,
        Primary,
        PrimaryLight
      };

      explicit ThemeImpl(Qt::Object* parent = nullptr);

      [[nodiscard]] invokable QColor color(int) const noexcept;
      [[nodiscard]] invokable QColor pcolor(int) const noexcept;

      friend class Theme;

    private:
      static void emplace(const Qt::String& folder) noexcept;
      static auto isSystemInDarkMode() noexcept -> bool;

      void load(const Qt::String& folder, const Qt::String& name) noexcept;

    private:
      ThemeMode m_dark_mode;
      map<ThemePalette, Qt::Color> m_dict_light;
      map<ThemePalette, Qt::Color> m_dict_dark;
      map<PrimaryPalette, ThemePalette> m_dict_primary;
      Qt::String m_folder;
      Qt::String m_name;

      map<Qt::String, ThemePalette> EnumerationDictionary;
  };
} // QtEx
Q_DECLARE_METATYPE(QtEx::ThemeImpl*)

namespace QtEx
{
  class Theme : public Qt::Object
  {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged FINAL)
    Q_PROPERTY(int darkMode READ darkMode WRITE setDarkMode NOTIFY darkModeChanged FINAL)
    Q_PROPERTY(ThemeImpl* io READ io NOTIFY ioChanged FINAL)

    public:
      enum ThemePalette
      {
        Rosewater,
        Flamingo,
        Pink,
        Mauve,
        Red,
        Maroon,
        Peach,
        Yellow,
        Green,
        Teal,
        Sky,
        Sapphire,
        Blue,
        Lavender,
        Text,
        Subtext1,
        Subtext0,
        Overlay2,
        Overlay1,
        Overlay0,
        Surface2,
        Surface1,
        Surface0,
        Base,
        Mantle,
        Crust
      };

      enum ThemeMode
      {
        Dark,
        Light
      };

      enum PrimaryPalette
      {
        PrimaryDark,
        Primary,
        PrimaryLight
      };

      Q_ENUM(ThemePalette)
      Q_ENUM(ThemeMode)
      Q_ENUM(PrimaryPalette)

      static auto get() -> Theme*;
      static auto create(QQmlEngine* qml_engine, QJSEngine* js_engine) -> Theme*;

      Theme(const Theme&) = delete;
      Theme(Theme&&) = delete;
      Theme& operator=(const Theme&) = delete;
      Theme& operator=(Theme&&) = delete;

      [[nodiscard]] Qt::String name() const;      void setName(const Qt::String&);
      [[nodiscard]] Qt::String folder() const;    void setFolder(const Qt::String&);
      [[nodiscard]] int darkMode() const;         void setDarkMode(int);
      [[nodiscard]] ThemeImpl* io() const;

    signals:
      void nameChanged();
      void folderChanged();
      void darkModeChanged();
      void ioChanged();

    private:
      explicit Theme(Qt::Object* parent = nullptr);
      bool eventFilter(Qt::Object* object, QEvent* event) override;

    private:
      ThemeImpl* m_io;
      unique_ptr<StaticConfig> m_config;
  };
} // QtEx

Q_DECLARE_METATYPE(QtEx::Theme*)