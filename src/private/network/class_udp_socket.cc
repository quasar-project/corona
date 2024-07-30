#include <corona/network/class_udp_socket.h>
#include <private/network/class_udp_socket_p.hh>

#include <bit>
#include <floppy/logging.h>

namespace llog = floppy::log;
namespace corona::network
{
  auto detail::is_fatal_error(std::error_code const& e, bool treat_unknown_as_fatal) -> bool {
    return e == std::errc::no_buffer_space
      or e == std::errc::not_enough_memory
      or e == std::errc::too_many_files_open
      or e == std::errc::too_many_files_open_in_system
      or e == std::errc::network_down
      or e == std::errc::network_unreachable
      or e == std::errc::no_protocol_option
      or e == std::errc::operation_not_permitted
      or e == std::errc::host_unreachable
      or e == std::errc::operation_not_supported
      or e == std::errc::timed_out
      or e == std::errc::connection_refused
      or e == std::errc::connection_reset
      or e == std::errc::bad_file_descriptor
      or e == std::errc::not_a_socket
      or e == std::errc::operation_canceled
      or e == std::errc::invalid_argument
      or (treat_unknown_as_fatal);
  }

  CUdpReceivingSocket::impl::impl(
    IBasicReceivingSocket::callback_receive_type callback,
    asio::io_context& io_context
  )
    : callback(std::move(callback))
    , buffer(std::array<u8, 1024>())
    , socket(io_context, asio::ip::udp::endpoint())
    , endpoint(this->socket.local_endpoint())
    , iodbg(io_context, 100ms)
  {}

  CUdpReceivingSocket::CUdpReceivingSocket(
    IBasicReceivingSocket::callback_receive_type callback,
    asio::io_context& io_context
  )
    : impl_(fl::make_box<impl>(std::move(callback), io_context)) {
    llog::trace("CUdpReceivingSocket: created blank udp socket");
  }

  CUdpReceivingSocket::~CUdpReceivingSocket() {
    this->CUdpReceivingSocket::stop();
  }

  auto CUdpReceivingSocket::start(
    u16 port,
    std::string_view remote_address,
    u16 remote_port
  ) -> result<> try {
    this->stop();
    this->impl_->endpoint = asio::ip::udp::endpoint(
      asio::ip::address::from_string(remote_address.data()),
      remote_port
    );
    this->impl_->socket.open(asio::ip::udp::v4());
    this->impl_->socket.bind(this->impl_->endpoint);
    this->read();
    llog::info("CUdpReceivingSocket: starting new udp connection on:");
    llog::info("CUdpReceivingSocket: -> rx: {}:{}", this->impl_->socket.local_endpoint().address().to_string(), port);
    llog::info("CUdpReceivingSocket: <- tx: TBD:TBD");
    this->impl_->has_error = false;
    return ok();
  } catch (std::exception const& e) {
    this->impl_->has_error = true;
    return error("CUdpReceivingSocket: failed to start udp socket: {}", e.what());
  }

  auto CUdpReceivingSocket::stop() -> void {
    this->impl_->socket.close();
    this->impl_->endpoint = {};
    llog::info("CUdpReceivingSocket: closing udp connection");
  }

  auto CUdpReceivingSocket::send(fl::bytearray_view data) -> void {
    auto buf = asio::buffer(data.data(), data.size());
    this->impl_->socket.send_to(buf, this->impl_->endpoint);
    this->impl_->iodbg(misc::socket_debug<256, f32>::Out, data.size_bytes());
  }

  auto CUdpReceivingSocket::rx_address_str() const -> std::string {
    return fmt::format("{}:{}",
      this->impl_->socket.local_endpoint().address().to_string(),
      this->impl_->socket.local_endpoint().port()
    );
  }

  auto CUdpReceivingSocket::tx_address_str() const -> std::string {
    return this->impl_->socket.local_endpoint() == this->impl_->endpoint
      ? "TBD"
      : fmt::format("{}:{}",
          this->impl_->endpoint.address().to_string(),
          this->impl_->endpoint.port()
        );
  }

  auto CUdpReceivingSocket::status() const -> IBasicReceivingSocket::Status {
    if(this->impl_->has_error)
      return Status::Error;
    if(this->rx_address_str() == this->tx_address_str())
      return Status::Unconnected;
    if(not this->impl_->socket.is_open())
      return Status::Closed;
    if(this->time_since_last_packet() > impl::DURATION_WARNING and this->time_since_last_packet() < impl::DURATION_TIMEOUT)
      return Status::HighDelay;
    if(this->time_since_last_packet() > impl::DURATION_TIMEOUT)
      return Status::Timeout;
    return Status::Ok;
  }

  auto CUdpReceivingSocket::time_since_last_packet() const -> std::chrono::steady_clock::duration {
    return std::chrono::steady_clock::now() - this->impl_->last_read;
  }

  auto CUdpReceivingSocket::debug_buffer() const -> misc::socket_debug<256, f32> const& {
    return this->impl_->iodbg;
  }

  auto CUdpReceivingSocket::read() -> void {
    this->impl_->last_read = std::chrono::steady_clock::now();
    this->impl_->socket.async_receive_from(
      asio::buffer(this->impl_->buffer),
      this->impl_->endpoint,
      [this](std::error_code const& error, usize bytes_transferred) -> void {
        this->impl_->iodbg(misc::socket_debug<256, f32>::In, bytes_transferred);
        if(error) {
          llog::error("CUdpReceivingSocket: failed to read udp socket: {}", error.message());
          return;
        }
        auto* const ptr = std::bit_cast<std::byte*>(this->impl_->buffer.data());
        this->impl_->callback(fl::bytearray(ptr, ptr + bytes_transferred)); // NOLINT(*-pro-bounds-pointer-arithmetic)
        this->read();
      }
    );
  }
} // namespace corona::network