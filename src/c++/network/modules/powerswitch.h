#pragma once

#include <leaf/global.h>

namespace network::modules
{
  using namespace leaf::types;

  class PowerSwitch
  {
    constexpr u32 REQUEST_MARKER = 0xAAAAAAAA;
    constexpr u16 LOCAL_PORT = 12000;

    struct [[gnu::packed]] RequestPacket
    {
      u32 marker;
      u16 command;
      u16 port;
      u16 checksum;
    };

    struct [[gnu::packed]] ResponsePacket
    {
      u16 header;
      u8 channel;
      u8 enabled;
      u32 voltage;
      u32 current;
    };

    public:

  };
}