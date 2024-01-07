//
// Created by user on 07.01.2024.
//

#include "logconfig.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace launcher
{
  using std::string_view;

  constexpr string_view DEFAULT_LOGGER_NAME = "logger";

  void LogFileConfiguration::init() const
  {
    auto logger = spdlog::rotating_logger_mt(
        DEFAULT_LOGGER_NAME.data(),
        string(this->filename),
        this->max_size,
        this->max_files
      );
    spdlog::set_default_logger(std::move(logger));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::debug);
  }
}
