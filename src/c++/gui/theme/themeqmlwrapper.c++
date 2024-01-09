//
// Created by whs31 on 09.01.2024.
//

#include "themeqmlwrapper.h"
#include "themeprovider.h"
#include <corona>
#include <config/config.h>
#include <QtCore/QDir>
#include <QtCore/QFile>

namespace gui::theme
{
  ThemeQMLWrapper::ThemeQMLWrapper(ThemeProvider* ptr, qt::Object* parent)
    : qt::Object(parent),
      m_ptr(ptr)
  {
    ThemeQMLWrapper::emplace();
  }

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
