//
// Created by whs31 on 25.10.2023.
//

#include <QtWidgets/QApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickWindow>
#include <QtQuickControls2/QQuickStyle>
#include <Libra/Windows>
#include <Libra/WinAPI>
#include <QtExtensions/QtExtensions>
#include <QtExtensions/Logging>
#include <QtExtensionsGeo/Register>

int main(int argc, char** argv)
{
  Libra::Windows::releaseConsole();

  QApplication app(argc, argv);
  QCoreApplication::setApplicationName(PROJECT_NAME);
  QCoreApplication::setApplicationVersion(PROJECT_VERSION);
  QCoreApplication::setOrganizationName(PROJECT_COMPANY);

  #ifndef Q_OS_WINDOWS
  QApplication::setWindowIcon(QIcon(":/icon.png"));
  #else
  QApplication::setWindowIcon(QIcon(":/icon.ico"));
  #endif

  llog(Info) "Starting" << QCoreApplication::applicationName() << "version" << QCoreApplication::applicationVersion();

  const QUrl qml_entry(QStringLiteral("qrc:/Main.qml"));

  Qtx::registerTypes();
  Qtx::Geo::registerQml();

  QQuickStyle::setStyle("Universal");

  QQmlEngine engine;
  QObject::connect(&engine, &QQmlEngine::quit, qApp, &QCoreApplication::quit);

  QQmlComponent component(&engine);
  QQuickWindow::setDefaultAlphaBuffer(true);
  component.loadUrl(qml_entry);
  if(component.isReady())
    component.create();
  else
    llog(Error) component.errorString();

  return QApplication::exec();
}