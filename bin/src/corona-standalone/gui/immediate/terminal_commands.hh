#pragma once

#include <array>
#include <string>
#include <vector>
#include <mutex>
#include <spdlog/sinks/base_sink.h>
#include <corona/vendored/imterm.h>

namespace corona::standalone::gui::immediate
{
  struct custom_command_struct {};

  class terminal_commands : public vendored::imterm::basic_spdlog_terminal_helper<terminal_commands, custom_command_struct, std::mutex> {
   public:
    terminal_commands();

    static auto no_completion(argument_type&) -> std::vector<std::string> { return {}; }

    static auto clear(argument_type&) -> void;
    static auto configure_term(argument_type&) -> void;
    static auto configure_term_autocomplete(argument_type&) -> std::vector<std::string>;
    static auto echo(argument_type&) -> void;
    static auto help(argument_type&) -> void;
    static auto quit(argument_type&) -> void;
  };
} // namespace corona::standalone::gui::immediate