#pragma once

#include <boost/asio.hpp>
#include <corona/network/modules/class_powerswitch.h>

namespace corona::network::modules
{
  struct CPowerSwitch::impl
  {
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
  };
} // namespace corona::network::modules