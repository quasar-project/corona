//
// Created by whs31 on 28.10.2023.
//

#include "theme.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtGui/QGuiApplication>
#include <QtExtensions/Logging>
#include <QtExtensions/Utility>

#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 0))
#include <QtGui/QStyleHints>
#elif ((QT_VERSION >= QT_VERSION_CHECK(6, 2, 1)))
#include <QtGui/qpa/qplatformtheme.h>
#include <QtGui/private/qguiapplication_p.h>
#else
#include <QtGui/QPalette>
#endif

namespace QtEx
{
  using Qt::Directory;
  using Qt::File;
  using Qt::String;

  ThemeImpl::ThemeImpl(Qt::Object* parent)
    : Qt::Object(parent)
    , m_dark_mode(Dark)
    , EnumerationDictionary({
      { "rosewater", Rosewater },
      { "flamingo", Flamingo },
      { "pink", Pink },
      { "mauve", Mauve },
      { "red", Red },
      { "maroon", Maroon },
      { "peach", Peach },
      { "yellow", Yellow },
      { "green", Green },
      { "teal", Teal },
      { "sky", Sky },
      { "sapphire", Sapphire },
      { "blue", Blue },
      { "lavender", Lavender },
      { "text", Text },
      { "subtext1", Subtext1 },
      { "subtext0", Subtext0 },
      { "overlay2", Overlay2 },
      { "overlay1", Overlay1 },
      { "overlay0", Overlay0 },
      { "surface2", Surface2 },
      { "surface1", Surface1 },
      { "surface0", Surface0 },
      { "base", Base },
      { "mantle", Mantle },
      { "crust", Crust }
    })
    , m_folder(QCoreApplication::applicationDirPath() + "/theme")
    , m_name("Catpuccin")
    , m_dict_primary({
      { PrimaryDark, Peach },
      { Primary, Peach },
      { PrimaryLight, Peach }
    })
  {
    qRegisterMetaType<ThemeImpl*>("ThemeImpl*");
  }

  QColor ThemeImpl::color(int key) const noexcept
  {
    switch(m_dark_mode)
    {
      case Light: return m_dict_light.at(static_cast<ThemeImpl::ThemePalette>(key));
      case Dark: [[fallthrough]];
      default: return m_dict_dark.at(static_cast<ThemeImpl::ThemePalette>(key));
    }
  }

  void ThemeImpl::emplace(const Qt::String& folder) noexcept
  {
    Utility::emplaceFile(String("%1/%2").arg(folder, QUrl(FALLBACK).fileName()), FALLBACK, Utility::EmplaceMode::OnlyIfMissing);
    llog(Debug) "Placed fallback file into folder";
  }

  void ThemeImpl::load(const String& folder, const String& name) noexcept
  {
    Directory directory(folder, "*.json", QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::Hidden);
    for(const auto& entry : directory.entryList())
    {
      File file(folder + "/" + entry);
      file.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
      if(not file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        llog(Error) "Failed to open file for read" << entry;
        continue;
      }

      auto doc = QJsonDocument::fromJson(file.readAll());
      file.close();

      if(doc.isNull() or not doc.isObject())
      {
        llog(Error) "Invalid JSON";
        continue;
      }
      auto json = doc.object();
      if(not json.contains("name"))
      {
        llog(Error) "Name is required for theme .json file";
        continue;
      }
      if(json["name"] != name)
        continue;

      {
        auto light = json["light"].toObject();
        for(auto it = light.begin(); it != light.end(); ++it)
          m_dict_light[EnumerationDictionary[it.key()]] = Qt::Color(it.value().toString());
      }

      {
        auto dark = json["dark"].toObject();
        for(auto it = dark.begin(); it != dark.end(); ++it)
          m_dict_dark[EnumerationDictionary[it.key()]] = Qt::Color(it.value().toString());
      }
    }
  }

  auto ThemeImpl::isSystemInDarkMode() noexcept -> bool
  {
    #if(QT_VERSION >= QT_VERSION_CHECK(6, 5, 0))
    return (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark);
    #elif((QT_VERSION >= QT_VERSION_CHECK(6, 2, 1)))
    if(const QPlatformTheme* const theme = QGuiApplicationPrivate::platformTheme())
       return(theme->appearance() == QPlatformTheme::Appearance::Dark);
    return false;
    #else
      QPalette palette = qApp->palette();
      QColor color = palette.color(QPalette::Window).rgb();
      return not (color.red() * 0.2126 + color.green() * 0.7152 + color.blue() * 0.0722 > 255 / 2);
    #endif
  }

  QColor ThemeImpl::pcolor(int x) const noexcept { return color(m_dict_primary.at(static_cast<ThemeImpl::PrimaryPalette>(x))); }

  auto Theme::get() -> Theme* { static Theme instance; return &instance; }
  auto Theme::create(QQmlEngine* qml_engine, QJSEngine* js_engine) -> Theme* { return get(); }

  Qt::String Theme::name() const { return io()->m_name; }
  void Theme::setName(const Qt::String& x)
  {
    io()->m_name = x;
    emit nameChanged();
    io()->load(folder(), name());
    emit ioChanged();
    m_config->set("name", Qt::Variant::fromValue(name()), StaticConfig::SetMode::WriteToFile);
  }

  Qt::String Theme::folder() const { return io()->m_folder; }
  void Theme::setFolder(const Qt::String& x)
  {
    io()->m_folder = x;
    emit folderChanged();
    ThemeImpl::emplace(folder());
    io()->load(folder(), name());
    emit ioChanged();
    m_config->set("folder", Qt::Variant::fromValue(folder()), StaticConfig::SetMode::WriteToFile);
  }

  int Theme::darkMode() const { return static_cast<int>(io()->m_dark_mode); }
  void Theme::setDarkMode(int x)
  {
    if(x == io()->m_dark_mode)
      return;
    io()->m_dark_mode = static_cast<ThemeImpl::ThemeMode>(static_cast<ThemeMode>(x));
    emit darkModeChanged();
    emit ioChanged();
    m_config->set("dark", Qt::Variant::fromValue(static_cast<bool>(darkMode() == Dark)), StaticConfig::SetMode::WriteToFile);
  }

  ThemeImpl* Theme::io() const { return m_io; }
  Theme::Theme(Qt::Object* parent)
    : Qt::Object(parent)
    , m_io(new ThemeImpl(this))
    , m_config(std::make_unique<StaticConfig>(QCoreApplication::applicationDirPath() + "/config/theme.json",
                                              ":/qtx/config/theme.json"))
  {
    io()->m_folder = QCoreApplication::applicationDirPath() + "/" + m_config->value<String>("folder");
    io()->m_name = m_config->value<String>("name");
    ThemeImpl::emplace(folder());
    io()->load(folder(), name());
    this->setDarkMode(m_config->value<bool>("dark") ? Dark : Light);
    QGuiApplication::instance()->installEventFilter(this);
  }

  bool Theme::eventFilter(Qt::Object* object, QEvent* event)
  {
    (void)object;
    if(event->type() == QEvent::ApplicationPaletteChange or event->type() == QEvent::ThemeChange)
    {
      setDarkMode(ThemeImpl::isSystemInDarkMode() ? ThemeMode::Dark : ThemeMode::Light);
      event->accept();
      return true;
    }
    return false;
  }
} // QtEx