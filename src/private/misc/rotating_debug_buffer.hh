#pragma once

#include <span>
#include <array>
#include <thread>
#include <chrono>
#include <type_traits>
#include <corona/detail/export.h>

namespace corona::misc
{
  template <usize N, typename T>
  struct rotating_debug_buffer
  {
    std::array<T, N> buf {};

    [[nodiscard]] inline auto operator[](usize index) -> T& { return this->buf[index]; }
    [[nodiscard]] inline auto operator[](usize index) const -> T const& { return this->buf[index]; }
    [[nodiscard]] inline auto size() const noexcept -> usize { return N; }
    [[nodiscard]] inline auto empty() const noexcept -> bool { return N == 0; }

    inline auto clear() -> void {
      this->buf.fill(T(0));
    }

    inline auto rotate() -> void {
      std::rotate(std::begin(this->buf), std::next(std::begin(this->buf)), std::end(this->buf));
      this->buf[N - 1] = T(0);
    }

    inline auto push(T val) -> void {
      this->buf[N - 1] += val;
    }
  };
} // namespace corona::misc