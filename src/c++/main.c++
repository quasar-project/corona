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
#include <QtExtensionsToolkit/Launcher>
#include "corona.h"

int main(int argc, char** argv)
{
  Corona corona;
  Qtx::ApplicationLauncher<QApplication> launcher(argc, argv, &corona);
  launcher.setProjectInfo(Qtx::ProjectInfo(PROJECT_NAME, PROJECT_VERSION, "Quasar Team", PROJECT_COMPANY));
  launcher.setQuickParameters({"qrc:/Main.qml", "Universal"});
  launcher.setIconPath(":/icon");
  launcher.setDumpPlatformInfo(true);
  launcher.setFreeConsole(false);
  launcher.launch();
  return QApplication::exec();
}