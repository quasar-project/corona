#pragma once

#include <string>
#include <leaf/global.h>

namespace network
{
  using std::string;
  using namespace leaf::types;

  struct Ipv4Address
  {
    string ip;

    [[nodiscard]] static auto local_address() -> expected<Ipv4Address, string>;
  };
}