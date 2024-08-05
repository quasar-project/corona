#pragma once

#include <utility>
#include <corona/network/detail/ipv4.h>

namespace corona
{
  class RemoteAddr : public formattable<RemoteAddr, char>
  {
   public:
    using ip_type = Ipv4;
    using port_type = u16;

    enum class LocalAddressType : char
    {
      AnyIpv4 = 0,
      Localhost = 1,
      LocalCached = 2
    };

    constexpr inline explicit RemoteAddr(RemoteAddr::ip_type ip, RemoteAddr::port_type port)
      : ip_(std::move(ip))
      , port_(port)
    {}

    explicit RemoteAddr(LocalAddressType type, RemoteAddr::port_type port);

    // todo: ctors
    // todo: ctor only taking port must initialize ip to either local addr or 0.0.0.0 depending on LocalAddressType

    virtual ~RemoteAddr() = default;
    RemoteAddr(RemoteAddr const&) = default;
    RemoteAddr(RemoteAddr&&) = default;
    auto operator=(RemoteAddr const&) -> RemoteAddr& = default;
    auto operator=(RemoteAddr&&) -> RemoteAddr& = default;

    [[nodiscard]] auto to_qstring() const -> ::QString;
    [[nodiscard]] virtual auto to_string() const -> string;

    [[nodiscard]] inline auto ip() const noexcept -> RemoteAddr::ip_type {
      return this->ip_;
    }

    [[nodiscard]] inline auto port() const noexcept -> RemoteAddr::port_type {
      return this->port_;
    }

    [[nodiscard]] static auto from_str(string_view str) -> result<RemoteAddr>;

   private:
    RemoteAddr::ip_type ip_;
    RemoteAddr::port_type port_;
  };
} // namespace corona