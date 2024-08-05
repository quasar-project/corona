#include <corona/network/detail/addr.h>

#include <corona/detail/logging.h>

namespace corona
{
  auto RemoteAddr::to_string() const -> string {
    return fmt::format("{}:{}", this->ip().to_string(), this->port());
  }

  auto RemoteAddr::from_str(string_view str) -> result<RemoteAddr> try {
    auto const pos = str.find(':');
    if(pos == string_view::npos)
      return error("invalid remote address string: {}", str);
    auto const ip = Ipv4::from_str(str.substr(0, pos));
    if(not ip)
      return error("invalid remote address string: {}", str);
    auto const port = std::stoi(str.substr(pos + 1));
    return ok(RemoteAddr{*ip, port});
  } catch(std::exception const& e) {
    return error("invalid remote address string: {}, reason: {}", str, e.what());
  }
} // namespace corona