#pragma once

#include <array>
#include <chrono>
#include <span>
#include <string>
#include <functional>
#include <corona/detail/export.h>

namespace boost::asio { class io_context; } // namespace boost::asio
namespace corona::misc { template <usize N, typename T> struct socket_debug; } // namespace corona::misc
namespace corona::network
{
  class CORONA_API ISocketBase
  {
   public:
    using SocketReceiveCallback = std::function<void(std::vector<std::byte>)>;

    enum class Status : char
    {
      Unconnected,
      HighDelay,
      Timeout,
      Error,
      Closed,
      Ok
    };

    virtual ~ISocketBase() = default;

    virtual auto start(u16 port) -> void = 0;
    virtual auto stop() -> void = 0;
    virtual auto send(std::span<std::byte const> data) -> void = 0;

    [[nodiscard]] virtual auto rx_address_str() const -> std::string = 0;
    [[nodiscard]] virtual auto tx_address_str() const -> std::string = 0;
    [[nodiscard]] virtual auto status() const -> Status = 0;
    [[nodiscard]] virtual auto time_since_last_packet() const -> std::chrono::steady_clock::duration = 0;
    [[nodiscard]] virtual auto debug_buffer() const -> misc::socket_debug<256, f32> const& = 0;

    [[nodiscard]]
    [[maybe_unused]]
    inline virtual auto time_since_last_packet_formatted() const -> std::string {
      return fmt::format("{:0>2}:{:0>2}:{:0>2}",
        std::chrono::duration_cast<std::chrono::hours>(this->time_since_last_packet()).count(),
        std::chrono::duration_cast<std::chrono::minutes>(this->time_since_last_packet()).count() % 60,
        std::chrono::duration_cast<std::chrono::seconds>(this->time_since_last_packet()).count() % 60
      );
    }

   protected:
    virtual auto read() -> void = 0;
  };
} // namespace corona::network