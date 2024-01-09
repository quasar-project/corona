//
// Created by whs31 on 09.01.2024.
//

#include "themeqmlwrapper.h"
#include "themeprovider.h"
#include <corona>
#include <config/config.h>
#include <config/configqmlwrapper.h>
#include <QtCore/QDir>
#include <QtCore/QFile>

constexpr std::string_view ERROR_COLOR = "#b162d8";

namespace gui::theme
{
  ThemeQMLWrapperInternal::ThemeQMLWrapperInternal(ThemeProvider* ptr, qt::Object* parent)
    : qt::Object(parent),
      m_ptr(ptr)
  {}

  auto ThemeQMLWrapperInternal::color(const qt::String& name) const -> qt::Color
  {
    return qt::Color(
      qt::String::fromStdString(
        string(
          m_ptr->color(name.toStdString())
            .map_error([name](const string& err){ llerror("failed to get theme color {} from wrapper, reason: {}",
              name,
              err
            );})
            .value_or(string(ERROR_COLOR))
        )
      )
    );
  }

  ThemeQMLWrapper::ThemeQMLWrapper(ThemeProvider* ptr, qt::Object* parent)
    : qt::Object(parent),
      m_ptr(ptr),
      m_internal(new ThemeQMLWrapperInternal(ptr, this))
  {
    ThemeQMLWrapper::emplace();
    qt::Object::connect(&application::Corona::ref().config_wrapper(), &config::ConfigQMLWrapper::valueChanged,
      this, [this](const qt::String& cat, const qt::String& name)
    {
      if(cat == "theme" and name == "theme_name")
        this->updateIO();
    });
  }

  auto ThemeQMLWrapper::mode() const -> PaletteType { return static_cast<PaletteType>(m_ptr->palette_type()); }
  void ThemeQMLWrapper::setMode(PaletteType mode)
  {
    m_ptr->set_palette_type(static_cast<gui::theme::ThemeProvider::PaletteType>(mode));
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
      .map_error([](const string& msg) { llerror("failed to initialize theme qml wrapper: {}", msg); });
    ensure(folder_name.has_value(), "theme folder name is not set");

    const auto folder = fmt::format("{}/{}", qt::Application::applicationDirPath(), folder_name.value());
    if(not qt::Dir(qt::String::fromStdString(folder)).exists())
    {
      lldebug("making theme folder {}...", folder_name.value());
      qt::Dir()
        .mkpath(qt::String::fromStdString(folder));
    }

    for(const unordered_map<qt::String, qt::String> themes = {
          { ":/theme/gruvbox.toml", qt::String::fromStdString(fmt::format("{}/gruvbox.toml", folder)) }
        }; const auto& [from, to] : themes)
    {
      if(not qt::File::exists(to))
      {
        lltrace("copying theme {} to {}...", from, to);
        qt::File::copy(from, to);
      }
    }
  }
}
