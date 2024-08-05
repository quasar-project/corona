#pragma once

#include <corona/detail/export.h>

namespace corona
{
  class Ipv4 : public formattable<Ipv4, char>
  {
   public:
    constexpr inline explicit Ipv4(u32 m)
      : m_(m)
    {}

    ~Ipv4() = default;
    Ipv4(Ipv4 const&) = default;
    Ipv4(Ipv4&&) = default;
    auto operator=(Ipv4 const&) -> Ipv4& = default;
    auto operator=(Ipv4&&) -> Ipv4& = default;

    [[nodiscard]] virtual auto to_string() const -> string;

    [[nodiscard]] constexpr inline auto as_u32() const noexcept -> u32 {
      return this->m_;
    }

    [[nodiscard]] static auto from_str(string_view str) -> result<Ipv4>;
    [[nodiscard]] static auto local_addresses() -> result<vector<Ipv4>>;
    [[nodiscard]] static auto local_address() -> result<Ipv4>;

   private:
    u32 m_;
  };
} // namespace corona