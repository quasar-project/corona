//
// Created by user on 06.01.2024.
//

#include "corona.h"
#include <QtGui/QFontDatabase>
#include <QtQml/QmlTypeAndRevisionsRegistration>
#include <config/config.h>
#include <config/configqmlwrapper.h>
#include <map/provider/providerqml.h>

namespace application
{
  using std::string;

  auto Corona::instance() -> Corona* { return dynamic_cast<Corona*>(qt::Application::instance()); }
  auto Corona::ref() -> Corona& { return *instance(); }

  Corona::Corona(int argc, char** argv)
    : qt::Application(argc, argv),
      m_config_wrapper(std::make_unique<::config::ConfigQMLWrapper>(this)),
      m_config(std::make_unique<::config::Config>([this](const string& cat, const string& name)
      {
        this->m_config_wrapper->value_changed_callback(cat, name);
      },
      config::ConfigQMLWrapper::create_default_config_callback))
  {
    this->m_config_wrapper->set_source_ptr(this->m_config.get());
    qt::Object::connect(
      this->m_config_wrapper.get(),
      &config::ConfigQMLWrapper::valueChanged,
      this,
      [](const qt::String& cat, const qt::String& name) { llinfo("config value changed: {}/{}", cat, name); }
    );
  }

  Corona::~Corona() = default;

  void Corona::start()
  {
    const auto font_id = qt::FontDatabase::addApplicationFont(":/fonts/Overpass.ttf");
    auto font_list = qt::FontDatabase::applicationFontFamilies(font_id);
    const auto family = font_list.first();
    qt::GuiApplication::setFont(qt::Font(family));

    this->m_config->load();
  }

  void Corona::register_types()
  {
    qmlRegisterModule("Corona.Config", 1, 0);
    qmlRegisterSingletonInstance<::config::ConfigQMLWrapper>("Corona.Config", 1, 0, "Config", this->m_config_wrapper.get());

    qmlRegisterModule("Corona.Map.Provider", 1, 0);
    qmlRegisterType<map::provider::OpenStreetMapProviderQML>("Corona.Map.Provider", 1, 0, "OpenStreetMapProvider");
  }

  auto Corona::config() const -> config::Config& { return *m_config; }
}
