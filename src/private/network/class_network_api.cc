#include <corona/network/class_network_api.h>

#include <chrono>
#include <floppy/logging.h>
#include <corona/network/modules/class_powerswitch.h>

using namespace std::chrono_literals;
namespace llog = floppy::log;

namespace corona::network
{
  CoronaAPI::CoronaAPI(boost::asio::io_context& io_context)
    : dirs_(meta::corona_meta)
    , config_("network.conf.yml", this->dirs().config_dir(), fl::saving_policy::autosave)
    , power_switch_(fl::make_box<modules::CPowerSwitch>(
        io_context,
        this->config(),
        [this](){ llog::trace("CPowerSwitch: on_channel_change"); },
        30s
      )
    )
  {
    llog::info("CoronaAPI: initialized");
  }

  CoronaAPI::~CoronaAPI() = default;

  auto CoronaAPI::dirs() const -> fl::filesystem::application_dirs const& { return this->dirs_; }
  auto CoronaAPI::dirs_mut() -> fl::filesystem::application_dirs& { return this->dirs_; }
  auto CoronaAPI::config() const -> CConfig const& { return this->config_; }
  auto CoronaAPI::config_mut() -> CConfig& { return this->config_; }
  auto CoronaAPI::power_switch() const -> modules::CPowerSwitch const& { return *this->power_switch_; }
  auto CoronaAPI::power_switch_mut() -> modules::CPowerSwitch& { return *this->power_switch_; }
} // namespace corona::network