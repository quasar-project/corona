//
// Created by whs31 on 25.10.2023.
//

#include <QtWidgets/QApplication>
#include <QtExtensionsToolkit/Launcher>
#include "corona.h"

int main(int argc, char** argv)
{
  Qtx::ApplicationLauncher<QApplication, Corona> launcher(argc, argv);
  launcher.setProjectInfo(Qtx::ProjectInfo(PROJECT_NAME, PROJECT_VERSION, PROJECT_COMPANY, PROJECT_DOMAIN));
  launcher.setQuickParameters({"qrc:/Main.qml", "Universal"});
  launcher.setIconPath(":/icon");
  launcher.setDumpPlatformInfo(true);
  launcher.setFreeConsole(false);
  return launcher.launch();
}