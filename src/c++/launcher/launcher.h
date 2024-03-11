#pragma once

#include <iostream>
#include <leaf/global.h>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickWindow>
#include <QtQuickControls2/QQuickStyle>
#include <QtWidgets/QApplication>
#include <application/base.h>
#include "logconfig.h"
#include "projectinfo.h"
#include "qtlogginghandler.h"
#include "qtquickoptions.h"

namespace launcher
{
  using std::string_view;
  using std::unique_ptr;

  template<typename T>
  concept IsApplicationBase = std::is_base_of_v<application::Base, T>;

  template<typename T>
  concept IsQtCoreApplication = std::is_base_of_v<QCoreApplication, T>;

  [[nodiscard]] inline auto platform_dependent_icon(const string_view stem) -> QIcon;
  [[nodiscard]] inline auto qml_entry_from_clean_path(const string_view path) -> QUrl;

  template<typename App>
  requires
    IsApplicationBase<App> and IsQtCoreApplication<App>
  class Launcher
  {
    public:
      Launcher(int argc,
               char** argv,
               ProjectInfo project_info,
               QtQuickOptions quick_options,
               const string_view icon_path,
               const LogFileConfiguration& log_config);

      [[nodiscard]] auto launch() -> int;

    private:
      unique_ptr<App> m_app;
      int m_argc;
      char** m_argv;
      ProjectInfo m_project_info;
      QtQuickOptions m_quick_options;
      string m_icon_path;
      unique_ptr<QQmlEngine> m_engine;
      unique_ptr<QQmlComponent> m_component;
  };
}

namespace launcher
{
  inline auto platform_dependent_icon(const string_view stem) -> QIcon
  {
    #if defined(Q_OS_WIN)
    return QIcon(QString::fromStdString(string(stem) + ".ico"));
    #else
    return QIcon(QString::fromStdString(string(stem) + ".png"));
    #endif
  }

  inline auto qml_entry_from_clean_path(const string_view path) -> QUrl
  {
    const auto full_path = fmt::format("qrc:/{}.qml", path);
    return { QString::fromStdString(full_path) };
  }

  template<typename App> requires IsApplicationBase<App> && IsQtCoreApplication<App>
  Launcher<App>::Launcher(
      int argc,
      char** argv,
      ProjectInfo project_info,
      QtQuickOptions quick_options,
      const string_view icon_path,
      const LogFileConfiguration& log_config
    )
      : m_argc(argc),
        m_argv(argv),
        m_project_info(std::move(project_info)),
        m_quick_options(std::move(quick_options)),
        m_icon_path(icon_path)
  {
    log_config.init();
    this->m_app.reset(new App(argc, argv));
  }

  template<typename App> requires IsApplicationBase<App> && IsQtCoreApplication<App>
  auto Launcher<App>::launch() -> int
  {
    #if 0
    FreeConsole();
    #endif

    App::setApplicationName(m_project_info.name.data());
    App::setApplicationVersion(m_project_info.version.data());
    App::setOrganizationName(m_project_info.organization.data());
    App::setOrganizationDomain(m_project_info.homepage.data());
    App::setWindowIcon(platform_dependent_icon(m_icon_path));

    llog::info("project {} version {} started", this->m_project_info.name, this->m_project_info.version);
    qInstallMessageHandler(logging_handler);

    if(this->m_quick_options.style == QtQuickStyle::Material)
      qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

    this->m_app->register_types();
    this->m_app->start();

    QQuickStyle::setStyle(QString::fromStdString(to_string(m_quick_options.style)));
    this->m_engine = std::make_unique<QQmlEngine>();
    QObject::connect(m_engine.get(), &QQmlEngine::quit, qApp, &QCoreApplication::quit);         // NOLINT(*-pro-type-static-cast-downcast)
    this->m_component = std::make_unique<QQmlComponent>(m_engine.get(), qml_entry_from_clean_path(m_quick_options.entry));
    QQuickWindow::setDefaultAlphaBuffer(true);
    this->m_component->loadUrl(qml_entry_from_clean_path(m_quick_options.entry));

    llog::trace("trying to create qml engine...");
    if(this->m_component->isReady())
      this->m_component->create();
    else
    {
      llog::error("{}", this->m_component->errorString().toStdString());
      std::cout << "press any key to exit" << std::endl;
      std::getchar();
      return 2;
    }
    llog::debug("launcher finished its task");

    return App::exec();
  }
}
