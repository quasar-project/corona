#include <corona/network/modules/class_powerswitch.h>
#include <private/network/modules/class_powerswitch_p.hh>

#include <bit>
#include <floppy/logging.h>
#include <private/misc/call_every.hh>

namespace llog = floppy::log;
namespace corona::network::modules
{
  CPowerSwitch::impl::impl(
    CPowerSwitch* parent,
    asio::io_context& io_context,
    CConfig const& config,
    std::chrono::steady_clock::duration request_interval,
    std::function<void()> on_channel_change
  )
    : q(parent)
    , config(config)
    , request_interval(request_interval)
    , socket(fl::make_box<CUdpReceivingSocket>(
        [this](fl::bytearray const& data) { this->q->read(data); },
        io_context
      )
    )
    , request_caller(
        io_context,
        request_interval,
        [this]() { this->q->request(); }
    )
    , channels(std::vector<ChannelData>(config.values().power_switch.channel_names.size()))
    , on_channel_change(std::move(on_channel_change))
  {}

  CPowerSwitch::CPowerSwitch(
    asio::io_context& io_context,
    CConfig const& config,
    std::function<void()> on_channel_change,
    std::chrono::steady_clock::duration request_interval
  )
    : impl_(fl::make_box<impl>(this, io_context, config, request_interval, std::move(on_channel_change))) {
    this->configure()
      .map_error([](std::string const& error) {
        llog::error("CPowerSwitch: {}", error);
      });
  }

  CPowerSwitch::~CPowerSwitch() {
    this->stop();
  }

  auto CPowerSwitch::frame() -> void {
    // todo
    throw std::logic_error("CPowerSwitch::frame: not implemented");
  }

  auto CPowerSwitch::channels() const -> std::vector<ChannelData> const& {
    return this->impl_->channels;
  }

  auto CPowerSwitch::config() const -> CConfig const& {
    return this->impl_->config;
  }

  auto CPowerSwitch::toggle(usize channel) -> void {
    auto const packet = impl::RequestPacket {
      .marker = impl::REQUEST_MARKER,
      .channel = static_cast<u16>(channel),
      .response_port = impl::LOCAL_PORT,
      .checksum = 0x0000
    };
    llog::debug("CPowerSwitch: toggling channel #{}", channel);
    llog::trace("CPowerSwitch: sending packet [{:x} {} {} {:x}]",
      packet.marker,
      packet.channel,
      packet.response_port,
      packet.checksum
    );
    this->write(fl::bytearray_view(std::bit_cast<std::byte const*>(&packet), sizeof(packet)));
  }

  auto CPowerSwitch::stop() -> void {
    llog::trace("CPowerSwitch: shutting down");
    this->impl_->socket->stop();
    this->impl_->request_caller.stop();
  }

  auto CPowerSwitch::restart(std::chrono::steady_clock::duration request_interval) -> void {
    this->impl_->request_interval = request_interval;
    this->configure()
      .map_error([](std::string const& error) {
        llog::error("CPowerSwitch: {}", error);
      });
  }

  auto CPowerSwitch::configure() -> result<> {
    this->impl_->request_caller.restart();
    return this->impl_->socket->start(
      impl::LOCAL_PORT,
      this->impl_->config.values().power_switch.address.ip,
      this->impl_->config.values().power_switch.address.port
    );
  }

  auto CPowerSwitch::request() -> void {
    this->toggle(impl::DUMMY_CHANNEL);
  }

  auto CPowerSwitch::read(fl::bytearray const& data) -> void {
    if(data.size() != sizeof(impl::ResponsePacket) * 8) {
      llog::warn("CPowerSwitch: invalid packet size: {} bytes, required at least {} bytes", data.size(), sizeof(impl::ResponsePacket) * 8);
      return;
    }
    auto* const response = std::bit_cast<std::array<impl::ResponsePacket, 8>*>(data.data());
    for(auto const& [
        marker,
        channel,
        enabled,
        voltage,
        current
      ] : *response) {
      this->impl_->channels[channel] = ChannelData {
        .voltage = static_cast<f32>(voltage) / 1'000.0F,
        .current = static_cast<f32>(current),
        .status = static_cast<bool>(enabled)
      };
    }
    this->impl_->on_channel_change();
  }

  auto CPowerSwitch::write(fl::bytearray_view data) -> void {
    this->impl_->socket->send(data);
  }
} // namespace corona::network::modules