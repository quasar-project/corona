//
// Created by whs31 on 25.10.2023.
//

#include <QtWidgets/QApplication>
#include <QtExtensionsToolkit/Launcher>
#include "corona.h"

int main(int argc, char** argv)
{
  Qtx::ApplicationLauncher<QApplication, Corona> launcher(
      argc, argv,
      {PROJECT_NAME, PROJECT_VERSION, PROJECT_COMPANY, PROJECT_DOMAIN},
      {"qrc:/Main.qml", "Universal"},
      ":/icon",
      Qtx::StartupMessage::DumpPlatformInfo,
      Qtx::ConsoleBehaviour::KeepConsole
  );
  return launcher.launch();
}