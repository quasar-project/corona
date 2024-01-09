//
// Created by user on 07.01.2024.
//

#include "logconfig.h"
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace launcher
{
  using std::vector;
  using std::shared_ptr;
  using std::string_view;

  constexpr string_view DEFAULT_LOGGER_NAME = ""; // NOLINT(*-redundant-string-init)

  void LogFileConfiguration::init() const
  {
    vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      string(this->filename),
      this->max_size,
      this->max_files
    ));
    auto combined_logger = std::make_shared<spdlog::logger>(DEFAULT_LOGGER_NAME.data(), std::begin(sinks), std::end(sinks));

    spdlog::set_default_logger(std::move(combined_logger));
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%=5!o] %^[%=7!l] %-20!s:%-3!#: %v %$");
    spdlog::flush_on(spdlog::level::debug);
  }
}
