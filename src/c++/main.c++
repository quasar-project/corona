//
// Created by whs31 on 25.10.2023.
//

#include <corona.h>
#include <launcher/launcher_base.h>

int main(int argc, char** argv)
{
  launcher::ApplicationLauncher<QApplication, Corona> app_launcher(
      argc, argv,
      { PROJECT_NAME, PROJECT_VERSION, PROJECT_COMPANY, PROJECT_DOMAIN },
      { "qrc:/Main.qml", "Material" },
      ":/icon",
      launcher::ConsoleBehaviour::KeepConsole // todo add cli arg
  );
  return app_launcher.launch();
}