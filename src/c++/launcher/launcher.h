//
// Created by user on 06.01.2024.
//

#pragma once

#include <afx>
#include <format>
#include <memory>
#include <utility>
#include <application/base.h>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickWindow>
#include <QtQuickControls2/QQuickStyle>
#include <QtWidgets/QApplication>
#include "project_info.h"
#include "qtquickoptions.h"
#include "qtlogginghandler.h"

namespace launcher
{
  using std::unique_ptr;
  using std::string_view;

  [[nodiscard]] inline auto platform_dependent_icon(const string_view stem) -> qt::Icon
  {
    #if defined(Q_OS_WIN)
    return qt::Icon(qt::String::fromStdString(string(stem) + ".ico"));
    #else
    return qt::Icon(qt::String::fromStdString(string(stem) + ".png"));
    #endif
  }

  [[nodiscard]] inline auto qml_entry_from_clean_path(const string_view path) -> qt::Url
  {
    const auto full_path = std::format("qrc:/{}.qml", path);
    return qt::Url(qt::String::fromStdString(full_path));
  }

  template<typename T>
  concept IsApplicationBase = std::is_base_of_v<application::Base, T>;

  template<typename T>
  concept IsQtCoreApplication = std::is_base_of_v<qt::CoreApplication, T>;

  template<typename App>
  requires
    IsApplicationBase<App> and IsQtCoreApplication<App>
  class Launcher
  {
    public:
      Launcher(int argc,
               char** argv,
               const ProjectInfo& project_info,
               const QtQuickOptions& quick_options,
               const string_view icon_path)
        : m_app(std::make_unique<App>(argc, argv)),
          m_argc(argc),
          m_argv(argv),
          m_project_info(project_info),
          m_quick_options(quick_options),
          m_icon_path(icon_path)
      {}

      [[nodiscard]] auto launch() -> int
      {
        #if 0
          FreeConsole();
        #endif

        App::setApplicationName(m_project_info.name.data());
        App::setApplicationVersion(m_project_info.version.data());
        App::setOrganizationName(m_project_info.organization.data());
        App::setOrganizationDomain(m_project_info.homepage.data());
        App::setWindowIcon(platform_dependent_icon(m_icon_path));

        if(this->m_quick_options.style == QtQuickStyle::Material)
          qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");

        this->m_app->register_types();
        this->m_app->start();
        qInstallMessageHandler(logging_handler);

        qt::QuickStyle::setStyle(qt::String::fromStdString(to_string(m_quick_options.style)));
        this->m_engine = std::make_unique<qt::QmlEngine>();
        qt::Object::connect(m_engine.get(), &qt::QmlEngine::quit, qApp, &qt::CoreApplication::quit);
        this->m_component = std::make_unique<qt::QmlComponent>(m_engine.get(), qml_entry_from_clean_path(m_quick_options.entry));
        qt::QuickWindow::setDefaultAlphaBuffer(true);
        this->m_component->loadUrl(qml_entry_from_clean_path(m_quick_options.entry));

        if(this->m_component->isReady())
          this->m_component->create();
        else
        {
          logging::error("{}", this->m_component->errorString());
          return 1;
        }

        return App::exec();
      }

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
