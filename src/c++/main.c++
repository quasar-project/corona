//
// Created by whs31 on 25.10.2023.
//

#include <corona>

import launcher;

// ReSharper disable once CppParameterMayBeConst
int main(int argc, char** argv)
{
  launcher::Launcher<application::Corona> app_launcher(
      argc, argv,
      { PROJECT_NAME, PROJECT_VERSION, PROJECT_COMPANY, PROJECT_DOMAIN },
      { "Main", launcher::QtQuickStyle::Material },
      ":/icon",
      {
        "logs/log.1",
        1024 * 1024 * 1024,
        1
      }
  );
  return app_launcher.launch();
}
