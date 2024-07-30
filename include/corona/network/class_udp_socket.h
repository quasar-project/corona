#pragma once

#include <corona/network/interface_socket.h>

namespace corona::network
{
  class CORONA_API CUdpReceivingSocket : public IBasicReceivingSocket
  {
   public:
    CUdpReceivingSocket(IBasicReceivingSocket::callback_receive_type callback, boost::asio::io_context& io_context);
    virtual ~CUdpReceivingSocket() override;

    [[nodiscard]] virtual auto start(
      u16 port,
      std::string_view remote_address,
      u16 remote_port
    ) -> result<> override;
    virtual auto stop() -> void override;
    virtual auto send(fl::bytearray_view data) -> void override;

    [[nodiscard]] virtual auto rx_address_str() const -> std::string override;
    [[nodiscard]] virtual auto tx_address_str() const -> std::string override;
    [[nodiscard]] virtual auto status() const -> IBasicReceivingSocket::Status override;
    [[nodiscard]] virtual auto time_since_last_packet() const -> std::chrono::steady_clock::duration override;
    [[nodiscard]] virtual auto debug_buffer() const -> misc::socket_debug<256, f32> const& override;

   protected:
    virtual auto read() -> void override;

   private:
    struct impl;
    fl::traits::pimpl<impl> impl_;
  };
} // namespace corona::network