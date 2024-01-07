#pragma once

#include <afx>
#include <string>

namespace launcher
{
  using std::string;

  struct LogFileConfiguration
  {
    void init() const;

    string filename;
    usize max_size;
    usize max_files;
  };
}