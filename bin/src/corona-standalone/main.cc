#include <floppy/floppy.h>
#include <corona-standalone/app/logger.hh>
#include <corona-standalone/app/app.hh>

#if defined(FLOPPY_OS_WINDOWS)
# include <windows.h>
# define main$ _main
#else
# define main$ main
#endif

namespace cs = corona::standalone;
auto main$(int argc, char** argv) -> int
{
  [[maybe_unused]] auto const logger = cs::app::Logger::make(
    cs::app::Logger::Params {
      .is_default = true,
      .name = "main",
      .level = cs::llog::level::level_enum::trace,
      .target = cs::app::Logger::Target::All,
      .pattern = "[%X] (%n) [%^%L%$] %^%v%$",
      .filename = "corona.log",
      .folder = std::filesystem::current_path() / "logs"
    }
  );

  return cs::app::Corona(argc, argv)
    .with_icon("icon")
    .with_quick_style(cs::app::QuickStyle::Material)
    .run_scene("Main");
}

#if defined(FLOPPY_OS_WINDOWS)
  auto __stdcall WinMain(
    [[maybe_unused]] ::HINSTANCE hinstance,
    [[maybe_unused]] ::HINSTANCE hprevinstance,
    [[maybe_unused]] ::LPSTR lpcmdline,
    [[maybe_unused]] int ncmdshow
  ) -> int {
    return main$(__argc, __argv);
  }
#endif