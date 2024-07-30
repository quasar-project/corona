#pragma once

#include <corona/detail/export.h>
#include <private/misc/rotating_debug_buffer.hh>

namespace corona::misc
{
  template <usize N, typename T>
  struct socket_debug
  {
    enum Direction
    {
      In,
      Out
    };

    enum class PlotType
    {
      Histogram,
      Line
    };
  };
} // namespace corona::misc