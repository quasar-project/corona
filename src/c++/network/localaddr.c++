#include <QtNetwork/QNetworkInterface>
#include <network/localaddr.h>

using leaf::Err;

namespace network
{
  auto local_address_impl() -> expected<Ipv4Address, string>
  {
    const auto interfaces = QNetworkInterface::allAddresses();
    for(const auto& interface : interfaces)
    {
      if(not interface.isLoopback() and interface.protocol() == QAbstractSocket::IPv4Protocol)
        return Ipv4Address {
          .ip = interface.toString().toStdString(),
        };
    }
    return Err("no addresses found for local interface between {} interfaces", interfaces.size());
  }

  auto Ipv4Address::local_address() -> expected<Ipv4Address, string>
  {
    static const auto local_address = local_address_impl();
    return local_address;
  }
}
