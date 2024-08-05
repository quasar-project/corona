#include <corona/network/detail/ipv4.h>

#include <ranges>
#include <qnetworkinterface.h>
#include <qhostaddress.h>
#include <corona/detail/logging.h>

namespace ranges = std::ranges;

namespace corona
{
  auto Ipv4::to_string() const -> std::string {
    return fmt::format("{}.{}.{}.{}",
      this->as_u32() >> 24,
      (this->as_u32() >> 16) & 0xFF,
      (this->as_u32() >> 8) & 0xFF,
      this->as_u32() & 0xFF
    );
  }

  auto Ipv4::from_str(string_view str) -> result<Ipv4> {
    auto success = true;
    auto const result = ::QHostAddress(str.data()).toIPv4Address(&success);
    if(not success)
      return error("failed to parse ipv4 address from string \'{}\'", str);
    return ok(Ipv4(result));
  }

  auto Ipv4::local_addresses() -> result<vector<Ipv4>> {
    auto const interfaces = ::QNetworkInterface::allAddresses()
      | ranges::views::filter([](auto const& interface) {
          return not interface.isLoopback() and interface.protocol() == QAbstractSocket::IPv4Protocol;
        })
      | ranges::views::transform([](auto const& interface) {
          auto const res = Ipv4::from_str(interface.toString().toStdString());
          if(not res) {
            llog::warn("Ipv4::local_address: {}", res.error());
            return Ipv4(0);
          }
          if(res->to_string() == "127.0.0.1")
            return Ipv4(0);
          return *res;
        })
      | ranges::views::filter([](auto const& interface) {
          return interface.as_u32() != 0;
        })
      | ranges::to<std::vector>();
    if(interfaces.empty())
      return error("no local ipv4 addresses found between {} interfaces", ::QNetworkInterface::allAddresses().size());
    for(auto const& interface : interfaces)
      llog::trace("Ipv4::local_address: found addr \'{}\'", interface);
    return ok(interfaces);
  }

  auto Ipv4::local_address() -> result<Ipv4> {
    return Ipv4::local_addresses()
      .map([](auto const& interfaces) {
        return interfaces.front();
      });
  }
} // namespace corona