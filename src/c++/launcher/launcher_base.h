//
// Created by whs31 on 27.10.2023.
//

#pragma once

#include <memory>
#include <type_traits>
#include <string>
#include <utility>
#include <qol/log>
#include <qol/winapi>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickWindow>
#include <QtQuickControls2/QQuickStyle>
#include "application_base.h"
#include "qapplication_selector.h"

namespace launcher
{
  using std::string;
  using std::string_view;
  using std::unique_ptr;

  struct ProjectInfo
  {
    ProjectInfo()
      : name("Unnamed")
      , version("0.0.0")
      , organization("Unspecified")
      , url("example.com")
    {
      llwarning("project information is not set");
    }

    ProjectInfo(const string_view name, const string_view version, const string_view organization, const string_view url)
      : name(name)
      , version(version)
      , organization(organization)
      , url(url)
    {}

    string name;
    string version;
    string organization;
    string url;
  };

  struct QuickParameters
  {
    QuickParameters()
      : entry(string())
      , style("Default")
    {}

    QuickParameters(const string_view entry, const string_view style)
      : entry(entry)
      , style(style)
    {}

    string entry;
    string style;
  };

  enum class ConsoleBehaviour : u8
  {
    ReleaseConsole,
    KeepConsole
  };

  template<typename T>
  concept IsApplicationBase = std::is_base_of_v<ApplicationBase, T>;

  template<typename T>
  concept IsQtCoreApplication = std::is_base_of_v<QCoreApplication, T>;

  template<typename T, typename U>
  requires
    IsApplicationBase<U> and IsQtCoreApplication<T>
  class ApplicationLauncher
  {
    public:
      explicit ApplicationLauncher(const int argc, char** argv,
                                   ProjectInfo info,
                                   QuickParameters parameters,
                                   Qt::String icon_path,
                                   const ConsoleBehaviour console_behaviour)
        : m_argc(argc)
        , m_argv(argv)
        , m_base(std::make_unique<U>())
        , m_project_info(std::move(info))
        , m_quick_parameters(std::move(parameters))
        , m_icon_path(std::move(icon_path))
        , m_free_console(console_behaviour)
      {}

      [[nodiscard]] ApplicationBase* base() const { return m_base.get(); }
      [[nodiscard]] int argc() const { return m_argc; }
      [[nodiscard]] char** argv() const { return m_argv; }

      [[nodiscard]] ProjectInfo projectInfo() const { return m_project_info; }
      void setProjectInfo(const ProjectInfo& x) { m_project_info = x; }

      [[nodiscard]] QuickParameters quickParameters() const { return m_quick_parameters; }
      void setQuickParameters(const QuickParameters& x) { m_quick_parameters = x; }

      [[nodiscard]] Qt::String iconPath() const { return m_icon_path; }
      void setIconPath(const Qt::String& x) { m_icon_path = x; }

      [[nodiscard]] ConsoleBehaviour freeConsole() const { return m_free_console; }
      void setFreeConsole(ConsoleBehaviour x) { m_free_console = x; }

      int launch() noexcept
      {
        if(freeConsole() == ConsoleBehaviour::ReleaseConsole)
          qol::windows::releaseConsole();

        m_app = std::make_unique<T>(m_argc, m_argv);
        T::setApplicationName(projectInfo().name.data());
        T::setApplicationVersion(projectInfo().version.data());
        T::setOrganizationName(projectInfo().organization.data());
        T::setOrganizationDomain(projectInfo().url.data());

        #ifndef LIBRA_OS_LINUX
        T::setWindowIcon(QIcon(iconPath() + ".png"));
        #else
        T::setWindowIcon(QIcon(iconPath() + ".ico"));
        #endif

        const QUrl QML_ENTRY(Qt::String(quickParameters().entry.data()));
        if(quickParameters().style == "Material")
          qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
        QQuickStyle::setStyle(quickParameters().style.data());

        base()->register_types();
        base()->start();

        m_engine = std::make_unique<QQmlEngine>();
        Qt::Object::connect(m_engine.get(), &QQmlEngine::quit, qApp, &QCoreApplication::quit);
        m_component = std::make_unique<QQmlComponent>(m_engine.get());
        QQuickWindow::setDefaultAlphaBuffer(true);
        m_component->loadUrl(QML_ENTRY);
        if(m_component->isReady())
          m_component->create();
        else
          llpanic("{}", m_component->errorString().toStdString());

        return T::exec();
      }

    private:
      unique_ptr<ApplicationBase> m_base;
      int m_argc;
      char** m_argv;
      ProjectInfo m_project_info;
      QuickParameters m_quick_parameters;
      Qt::String m_icon_path;
      ConsoleBehaviour m_free_console;
      unique_ptr<T> m_app;
      unique_ptr<QQmlEngine> m_engine;
      unique_ptr<QQmlComponent> m_component;
  };
}
