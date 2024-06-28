#include <floppy/floppy.h>
#include <floppy/logging.h>

#if defined(FLOPPY_OS_WINDOWS)
# include <windows.h>
# define main$ _main
#else
# define main$ main
#endif

auto main$(int argc, char** argv) -> int
{
  std::cout << "Hello, World!" << std::endl;
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