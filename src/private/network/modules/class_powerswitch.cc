#include <corona/network/modules/class_powerswitch.h>
#include <private/network/modules/class_powerswitch_p.hh>

#include <private/misc/call_every.hh>

namespace corona::network::modules
{
  CPowerSwitch::impl::impl(
    asio::io_context& io_context,
    CConfig const& config,
    std::chrono::steady_clock::duration request_interval
  )
    : config(config)
    , request_interval(request_interval)
    , socket(fl::make_box<CUdpReceivingSocket>(
        [this](fl::bytearray const& data) { this->read(data); },
        io_context
      )
    )
    , request_caller(
        io_context,
        request_interval,
        [this]() { this->request(); }
    )
    , channels(std::vector<ChannelData>(config.values().power_switch.channel_names.size()))
  {
  }

  auto CPowerSwitch::impl::read(floppy::bytearray const& data) -> void {

  }

  auto CPowerSwitch::impl::request() -> void {}

  auto CPowerSwitch::request() -> void {
    this->toggle(impl::DUMMY_CHANNEL);
  }
} // namespace corona::network::modules