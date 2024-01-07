//
// Created by whs31 on 25.10.2023.
//

#include <corona>


#if defined CXX20_MODULES_DISABLED
#include <launcher/launcher.h>
#else
import launcher;
#endif

// ReSharper disable once CppParameterMayBeConst
int main(int argc, char** argv)
{
  launcher::Launcher<application::Corona> app_launcher(
      argc, argv,
      { PROJECT_NAME, PROJECT_VERSION, PROJECT_COMPANY, PROJECT_DOMAIN },
      { "Main", launcher::QtQuickStyle::Material },
      ":/icon",
      {
        .filename = "logs/log.1",
        .max_size = 1024 * 1024 * 1024,
        .max_files = 1
      }
  );
  return app_launcher.launch();
}
