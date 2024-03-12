//
// Created by user on 06.01.2024.
//

#include <memory>
#include <leaf/global.h>
#include <QtGui/QFontDatabase>
#include <QtQml/QmlTypeAndRevisionsRegistration>
#include <config/configqmlwrapper.h>
#include <map/provider/providerqml.h>
#include <gui/theme/themeprovider.h>
#include <gui/theme/themeqmlwrapper.h>
#include <gui/theme/circular_reveal.h>
#include <network/localaddr.h>
#include <network/modules/powerswitch.h>
#include <application/corona.h>

template<>
struct fmt::formatter<QString> : fmt::formatter<std::string>
{
  auto format(const QString& str, format_context& ctx) -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", str.toStdString());
  }
};

namespace application
{
  using std::shared_ptr;
  using std::string;
  using std::string_view;
  using namespace std::string_view_literals;
  using namespace leaf::types;

  constexpr auto DEFAULT_THEME_FOLDER = "themes"sv;
  constexpr auto DEFAULT_THEME_NAME = "Gruvbox"sv;
  constexpr auto DEFAULT_PALETTE_TYPE = "dark"sv;

  auto Corona::instance() -> Corona* { return dynamic_cast<Corona*>(QApplication::instance()); }
  auto Corona::ref() -> Corona& { return *instance(); }

  Corona::Corona(int argc, char** argv)
    : QApplication(argc, argv),
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
      )),
      m_main_config(std::make_unique<leaf::conf::Config<::config::ConfigData>>(config::build_config())),
      m_network_api(std::make_shared<constellation::network::NetworkAPI>())
  {
    this->m_config_wrapper->set_source_ptr(this->m_config.get());
    QObject::connect(
      this->m_config_wrapper.get(),
      &config::ConfigQMLWrapper::valueChanged,
      this,
      [](const QString& cat, const QString& name) { llog::info("config value changed: {}/{}", cat, name); }
    );

    // this is in constructor because it will be needed by other classes earlier than start()
    this->m_config->load();
  }

  Corona::~Corona() = default;

  auto Corona::start() -> void
  {
    const auto font_id = QFontDatabase::addApplicationFont(":/fonts/Overpass.ttf");
    auto font_list = QFontDatabase::applicationFontFamilies(font_id);
    const auto family = font_list.first();
    QGuiApplication::setFont(QFont(family));

    this->m_theme_provider->load();
  }

  auto Corona::register_types() -> void
  {
    auto theme = new ::gui::theme::ThemeQMLWrapper(m_theme_provider.get(), this);

    qmlRegisterModule("Corona.Config", 1, 0);
    qmlRegisterSingletonInstance<::config::ConfigQMLWrapper>("Corona.Config", 1, 0, "Config", this->m_config_wrapper.get());

    qmlRegisterModule("Corona.Map.Provider", 1, 0);
    qmlRegisterType<map::provider::OpenStreetMapProviderQML>("Corona.Map.Provider", 1, 0, "OpenStreetMapProvider");

    qmlRegisterModule("Corona.Gui.Theme", 1, 0);
    qmlRegisterSingletonInstance<::gui::theme::ThemeQMLWrapper>("Corona.Gui.Theme", 1, 0, "Theme", theme);
    qmlRegisterType<::gui::theme::CircularReveal>("Corona.Gui.Theme", 1, 0, "ThemeCircularPaletteRevealInternal");

    qmlRegisterSingletonInstance<::constellation::network::NetworkAPI>("io.quasar.constellation.network", 1, 0, "NetworkAPI", this->m_network_api.get());
  }

  auto Corona::config() const -> config::Config& { return *m_config; }
  auto Corona::config_wrapper() const -> config::ConfigQMLWrapper& { return *m_config_wrapper; }
  auto Corona::main_config() const -> leaf::conf::Config<config::ConfigData>& { return *m_main_config; }
}
