#pragma once

#include <afx>
#include <string>
#include <functional>

namespace config
{
  using std::string;
  using std::string_view;

  constexpr string_view CONFIG_DIRECTORY_NAME = "config";
  constexpr string_view CONFIG_FILENAME = "config.yaml";

  class Config
  {
    using ValueChangedCB = std::function<void (string)>;
  };
}
