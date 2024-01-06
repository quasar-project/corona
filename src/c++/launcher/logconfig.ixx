module;

#include <afx>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

export module launcher.logconfig;

using namespace std;

namespace launcher
{
  export struct LogFileConfiguration
  {
    LogFileConfiguration(const string_view filename, const usize max_size, const usize max_files)
      : filename(filename),
        max_size(max_size),
        max_files(max_files)
    {}

    void init() const
    {
      auto logger = spdlog::rotating_logger_mt(
        "logger",
        string(this->filename),
        this->max_size,
        this->max_files
      );
      spdlog::set_default_logger(std::move(logger));
      spdlog::set_level(spdlog::level::trace);
      spdlog::flush_on(spdlog::level::debug);
    }

    private:
      string filename;
      usize max_size;
      usize max_files;
  };
}
