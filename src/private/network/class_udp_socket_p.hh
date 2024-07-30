#pragma once

#include <system_error>
#include <corona/network/class_udp_socket.h>
#include <private/misc/socket_debug.hh>

using namespace std::chrono_literals;
namespace asio = boost::asio;
namespace corona::network
{
  namespace detail
  {
    [[nodiscard]] auto is_fatal_error(std::error_code const& e, bool treat_unknown_as_fatal) -> bool;
  } // namespace detail

  struct CUdpReceivingSocket::impl
  {
    static constexpr auto DURATION_TIMEOUT = 10s;
    static constexpr auto DURATION_WARNING = 500ms;

    impl(IBasicReceivingSocket::callback_receive_type callback, boost::asio::io_context& io_context);

    IBasicReceivingSocket::callback_receive_type callback;
    std::array<u8, 1024> buffer;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint endpoint;
    std::chrono::steady_clock::time_point last_read = std::chrono::steady_clock::now();
    bool has_error = false;
    misc::socket_debug<256, f32> iodbg;
  };
} // namespace corona::network