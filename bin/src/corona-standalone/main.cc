#include <corona-standalone/app/class_logger.hh>
#include <corona-standalone/app/app.hh>

#if defined(FLOPPY_OS_WINDOWS)
# include <windows.h>
# define main$ _main
#else
# define main$ main
#endif

auto main$(int argc, char** argv) -> int
{
  using corona::standalone::app::CLogger;
  using corona::standalone::app::Corona;
  using corona::standalone::app::QuickStyle;
  using namespace corona::standalone::llog::level;

  auto logger = CLogger::make(
    CLogger::Params {
      .is_default = true,
      .name = "main",
      .level = trace,
      .target = CLogger::Target::All,
      .pattern = "[%X] (%n) [%^%L%$] %^%v%$",
      .filename = "corona.log",
      .folder = std::filesystem::current_path() / "logs"
    }
  );

  return Corona(argc, argv, logger)
    .with_icon("resources/icon")
    .with_quick_style(QuickStyle::Material)
    .run_scene("qml/Main");
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