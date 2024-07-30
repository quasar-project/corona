#pragma once

#include <boost/asio.hpp>
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

    misc::call_every<std::function<void()>> request_caller;
  };
} // namespace corona::network::modules