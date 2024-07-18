#pragma once

#include <array>
#include <string>
#include <vector>
#include <mutex>
#include <spdlog/sinks/base_sink.h>
#include <imterm/terminal.hpp>
#include <imterm/terminal_helpers.hpp>

namespace corona::standalone::gui::immediate
{
  struct custom_command_struct {
    bool should_close = false;
  };

  class terminal_commands : public ImTerm::basic_spdlog_terminal_helper<terminal_commands, custom_command_struct, std::mutex> {
   public:
    terminal_commands();

    static std::vector<std::string> no_completion(argument_type&) { return {}; }

    static void clear(argument_type&);
    static void configure_term(argument_type&);
    static std::vector<std::string> configure_term_autocomplete(argument_type&);
    static void echo(argument_type&);
    static void exit(argument_type&);
    static void help(argument_type&);
    static void quit(argument_type&);
  };
} // namespace corona::standalone::gui::immediate