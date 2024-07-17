#pragma once

#include <filesystem>

namespace tests
{
  namespace fs = std::filesystem;

  [[nodiscard]] auto data(std::string_view fname) -> fs::path {
    return fs::current_path() / "data" / fname;
  }
} // namespace tests