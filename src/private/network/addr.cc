#include <corona/network/detail/addr.h>

#include <qstring.h>
#include <corona/detail/logging.h>

namespace
{
  using namespace corona;

  [[nodiscard]] auto make_ipv4(RemoteAddr::LocalAddressType type) -> Ipv4 {
    switch(type) {
      case RemoteAddr::LocalAddressType::AnyIpv4:
        return Ipv4::any();
      case RemoteAddr::LocalAddressType::Localhost:
        return Ipv4::localhost();
      case RemoteAddr::LocalAddressType::LocalCached:
        return Ipv4::local_address_cached();
      default: fl::unreachable();
    }
  }
} // namespace

namespace corona
{
  RemoteAddr::RemoteAddr(RemoteAddr::LocalAddressType type, RemoteAddr::port_type port)
    : ip_(::make_ipv4(type))
    , port_(port)
  {}

  auto RemoteAddr::to_qstring() const -> ::QString {
    return ::QString::fromStdString(this->to_string());
  }

  auto RemoteAddr::to_string() const -> string {
    return fmt::format("{}:{}", this->ip().to_string(), this->port());
  }

  auto RemoteAddr::from_str(string_view str) -> result<RemoteAddr> try {
    auto const pos = str.find(':');
    if(pos == string_view::npos)
      return error("invalid remote address string: {}", str);
    auto const ip = RemoteAddr::ip_type::from_str(str.substr(0, pos));
    if(not ip)
      return error("invalid remote address string: {}", str);
    auto const port = std::stoi(std::string(str.substr(pos + 1)));
    return ok(RemoteAddr{*ip, static_cast<u16>(port)});
  } catch(std::exception const& e) {
    return error("invalid remote address string: {}, reason: {}", str, e.what());
  }
} // namespace corona