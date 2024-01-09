//
// Created by user on 06.01.2024.
//

#include "corona.h"
#include <QtGui/QFontDatabase>
#include <QtQml/QmlTypeAndRevisionsRegistration>
#include <config/config.h>
#include <config/configqmlwrapper.h>
#include <map/provider/providerqml.h>
#include <gui/theme/themeprovider.h>
#include <gui/theme/themeqmlwrapper.h>

namespace application
{
  using std::string;
  using std::string_view;
  using namespace std::string_view_literals;

  constexpr auto DEFAULT_THEME_FOLDER = "themes"sv;
  constexpr auto DEFAULT_THEME_NAME = "Gruvbox"sv;
  constexpr auto DEFAULT_PALETTE_TYPE = "dark"sv;

  auto Corona::instance() -> Corona* { return dynamic_cast<Corona*>(qt::Application::instance()); }
  auto Corona::ref() -> Corona& { return *instance(); }

  Corona::Corona(int argc, char** argv)
    : qt::Application(argc, argv),
      m_config_wrapper(std::make_unique<::config::ConfigQMLWrapper>(this)),
      m_config(std::make_unique<::config::Config>([this](const string& cat, const string& name)
      {
        this->m_config_wrapper->value_changed_callback(cat, name);
      },
      config::ConfigQMLWrapper::create_default_config_callback)),
      m_theme_provider(std::make_unique<::gui::theme::ThemeProvider>(
        m_config->get<string>("theme", "folder")
          .value_or(string(DEFAULT_THEME_FOLDER)),
        m_config->get<string>("theme", "name")
          .value_or(string(DEFAULT_THEME_NAME)),
          gui::theme::ThemeProvider::palette_from_string_view(
            m_config->get<string>("theme", "palette")
              .value_or(string(DEFAULT_PALETTE_TYPE))
            )
      ))
  {
    this->m_config_wrapper->set_source_ptr(this->m_config.get());
    qt::Object::connect(
      this->m_config_wrapper.get(),
      &config::ConfigQMLWrapper::valueChanged,
      this,
      [](const qt::String& cat, const qt::String& name) { llinfo("config value changed: {}/{}", cat, name); }
    );

    // this is in constructor because it will be needed by other classes earlier than start()
    this->m_config->load();
  }

  Corona::~Corona() = default;

  void Corona::start()
  {
    const auto font_id = qt::FontDatabase::addApplicationFont(":/fonts/Overpass.ttf");
    auto font_list = qt::FontDatabase::applicationFontFamilies(font_id);
    const auto family = font_list.first();
    qt::GuiApplication::setFont(qt::Font(family));

    this->m_theme_provider->load();
  }

  void Corona::register_types()
  {
    auto theme = new ::gui::theme::ThemeQMLWrapper(m_theme_provider.get(), this);

    qmlRegisterModule("Corona.Config", 1, 0);
    qmlRegisterSingletonInstance<::config::ConfigQMLWrapper>("Corona.Config", 1, 0, "Config", this->m_config_wrapper.get());

    qmlRegisterModule("Corona.Map.Provider", 1, 0);
    qmlRegisterType<map::provider::OpenStreetMapProviderQML>("Corona.Map.Provider", 1, 0, "OpenStreetMapProvider");

    qmlRegisterModule("Corona.Gui.Theme", 1, 0);
    qmlRegisterSingletonInstance<::gui::theme::ThemeQMLWrapper>("Corona.Gui.Theme", 1, 0, "Theme", theme);
  }

  auto Corona::config() const -> config::Config& { return *m_config; }
  auto Corona::config_wrapper() const -> config::ConfigQMLWrapper& { return *m_config_wrapper; }
}
