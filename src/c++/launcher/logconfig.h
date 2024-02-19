#pragma once

#include <string>
#include <leaf/global.h>

namespace launcher
{
  using std::string;
  using namespace leaf::types;

  struct LogFileConfiguration
  {
    void init() const;

    string filename;
    usize max_size;
    usize max_files;
  };
}