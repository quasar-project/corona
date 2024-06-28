#include <floppy/floppy.h>
#include <corona-standalone/app/logger.hh>
#include <corona-standalone/app/iapplication.hh>

#if defined(FLOPPY_OS_WINDOWS)
# include <windows.h>
#define main$ _main
#else
# define main$ main
#endif

namespace cs = corona::standalone;
auto main$(int argc, char** argv) -> int
{
  [[maybe_unused]] auto const logger = cs::app::Logger(
    cs::app::Logger::Params{
      .is_default = true,
      .name = "main",
      .level = cs::llog::level::level_enum::trace,
      .target = cs::app::Logger::Target::All,
      .pattern = "[%Y-%m-%d %H:%M:%S.%e] (%n) [%^%l%$] %v",
      .filename = "corona.log",
      .folder = std::filesystem::current_path() / "logs"
    }
  );
  cs::llog::trace("example trace message");
  cs::llog::debug("example debug message");
  cs::llog::info("example info message");
  cs::llog::warn("example warn message");
  cs::llog::error("example error message");
  cs::llog::critical("example critical message");
  std::cin.get();
  return 0;
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