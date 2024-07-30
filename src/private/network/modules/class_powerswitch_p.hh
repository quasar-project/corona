#pragma once

#include <boost/asio.hpp>
#include <corona/network/class_udp_socket.h>
#include <corona/network/modules/class_powerswitch.h>
#include <private/misc/call_every.hh>

namespace asio = boost::asio;
namespace corona::network::modules
{
  struct CPowerSwitch::impl
  {
    static constexpr auto LOCAL_PORT = u16(12000); // NOLINT(*-readability-casting)
    static constexpr auto DUMMY_CHANNEL = u16(9'999); // NOLINT(*-readability-casting)
    static constexpr auto REQUEST_MARKER = u32(0xAAAAAAAA); // NOLINT(*-readability-casting)

    #pragma pack(push, 1)
    struct [[gnu::packed]] RequestPacket
    {
      u32 marker;
      u16 channel;
      u16 response_port;
      u16 checksum;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct [[gnu::packed]] ResponsePacket
    {
      u16 marker;
      u8 channel;
      u8 enabled;
      u32 voltage; ///< Напряжение в милливольтах
      u32 current; ///< Ток в миллиамперах
    };
    #pragma pack(pop)

    CConfig const& config;
    std::chrono::steady_clock::duration request_interval;
    fl::box<IBasicReceivingSocket> socket;
    misc::call_every<std::function<void()>> request_caller;
    std::vector<ChannelData> channels;

    impl(
      asio::io_context& io_context,
      CConfig const& config,
      std::chrono::steady_clock::duration request_interval
    );

    auto read(fl::bytearray const& data) -> void;
    auto request() -> void;
  };
} // namespace corona::network::modules