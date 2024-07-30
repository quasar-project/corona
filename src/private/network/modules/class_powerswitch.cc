#include <corona/network/modules/class_powerswitch.h>
#include <private/network/modules/class_powerswitch_p.hh>

#include <private/misc/call_every.hh>

namespace corona::network::modules
{
  auto CPowerSwitch::request() -> void {
    this->toggle(impl::DUMMY_CHANNEL);
  }
} // namespace corona::network::modules