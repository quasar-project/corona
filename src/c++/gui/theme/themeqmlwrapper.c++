//
// Created by whs31 on 09.01.2024.
//

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <corona>
#include <config/config.h>
#include <config/configqmlwrapper.h>
#include <gui/theme/themeprovider.h>
#include <gui/theme/themeqmlwrapper.h>

template<>
struct fmt::formatter<QString> : fmt::formatter<std::string>
{
  auto format(const QString& str, format_context& ctx) -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", str.toStdString());
  }
};

constexpr std::string_view ERROR_COLOR = "#b162d8";

namespace gui::theme
{
  ThemeQMLWrapperInternal::ThemeQMLWrapperInternal(ThemeProvider* ptr, QObject* parent)
    : QObject(parent),
      m_ptr(ptr)
  {}

  auto ThemeQMLWrapperInternal::color(const QString& name) const -> QColor
  {
    return QColor(
      QString::fromStdString(
        string(
          m_ptr->color(name.toStdString())
            .map_error([name](const string& err){ llog::error("failed to get theme color {} from wrapper, reason: {}",
              name,
              err
            );})
            .value_or(string(ERROR_COLOR))
        )
      )
    );
  }

  ThemeQMLWrapper::ThemeQMLWrapper(ThemeProvider* ptr, QObject* parent)
    : QObject(parent),
      m_ptr(ptr),
      m_internal(new ThemeQMLWrapperInternal(ptr, this))
  {
    ThemeQMLWrapper::emplace();
    QObject::connect(&application::Corona::ref().config_wrapper(), &config::ConfigQMLWrapper::valueChanged,
      this, [this](const QString& cat, const QString& name)
    {
      if(cat == "theme" and name == "theme_name")
        this->updateIO();
    });
  }

  void ThemeQMLWrapper::toggle()
  {
    llog::trace("toggling theme");
    if(this->mode() == PaletteType::Light)
      this->setMode(PaletteType::Dark);
    else
      this->setMode(PaletteType::Light);
  }

  auto ThemeQMLWrapper::mode() const -> PaletteType { return static_cast<PaletteType>(m_ptr->palette_type()); }
  void ThemeQMLWrapper::setMode(PaletteType mode)
  {
    m_ptr->set_palette_type(static_cast<gui::theme::ThemeProvider::PaletteType>(mode));
    emit ioChanged();
    emit modeChanged();
  }

  auto ThemeQMLWrapper::io() const -> ThemeQMLWrapperInternal* { return this->m_internal; }
  void ThemeQMLWrapper::setIO(ThemeQMLWrapperInternal* io)
  {
    this->m_internal = io;
    emit ioChanged();
  }

  void ThemeQMLWrapper::updateIO() { emit ioChanged(); }

  void ThemeQMLWrapper::emplace()
  {
    const auto folder_name = application::Corona::ref()
      .config()
      .get<string>("theme", "folder")
      .map_error([](const string& msg) { llog::error("failed to initialize theme qml wrapper: {}", msg); });
    if(not folder_name) {
      return;
    }

    const auto folder = fmt::format("{}/{}", QApplication::applicationDirPath(), folder_name.value());
    if(not QDir(QString::fromStdString(folder)).exists())
    {
      llog::debug("making theme folder {}...", folder_name.value());
      QDir().mkpath(QString::fromStdString(folder));
    }

    for(const unordered_map<QString, QString> themes = {
          { ":/theme/gruvbox.toml", QString::fromStdString(fmt::format("{}/gruvbox.toml", folder)) }
        }; const auto& [from, to] : themes)
    {
      if(not QFile::exists(to))
      {
        llog::trace("copying theme {} to {}...", from, to);
        QFile::copy(from, to);
      }
    }
  }
}
