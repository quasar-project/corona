#pragma once

#include <corona/network/detail/ipv4.h>

namespace corona
{
  class RemoteAddr : public formattable<RemoteAddr, char>
  {
   public:
    enum class LocalAddressType : char
    {
      AnyIpv4 = 0,
      Localhost = 1,
      LocalCached = 2
    };

    // todo: ctors
    // todo: ctor only taking port must initialize ip to either local addr or 0.0.0.0 depending on LocalAddressType

    virtual ~RemoteAddr() = default;
    RemoteAddr(RemoteAddr const&) = default;
    RemoteAddr(RemoteAddr&&) = default;
    auto operator=(RemoteAddr const&) -> RemoteAddr& = default;
    auto operator=(RemoteAddr&&) -> RemoteAddr& = default;

    [[nodiscard]] virtual auto to_string() const -> string;

    [[nodiscard]] inline auto ip() const noexcept -> Ipv4 {
      return this->ip_;
    }

    [[nodiscard]] inline auto port() const noexcept -> u16 {
      return this->port_;
    }

    [[nodiscard]] static auto from_str(string_view str) -> result<RemoteAddr>;

   private:
    Ipv4 ip_;
    u16 port_;
  };
} // namespace corona