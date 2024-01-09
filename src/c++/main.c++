//
// Created by whs31 on 25.10.2023.
//

#include <corona>

#if defined CXX20_MODULES_DISABLED
#include <launcher/launcher.h>
#else
import launcher;
#endif

/*
 * @ TODO LIST:
 * - Make provider offline path from config
 */

/**
 * Main function that launches the application.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of command line arguments.
 * @return The exit code of the application.
 */
// ReSharper disable once CppParameterMayBeConst
auto main(int argc, char** argv) -> int
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
