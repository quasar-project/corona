#pragma once

#include <floppy/directories.h>
#include <corona/config.h>
#include <corona/detail/export.h>

namespace boost::asio { class io_context; } // namespace boost::asio
namespace corona::network
{
  namespace modules
  {
    class CPowerSwitch;
  } // namespace modules

  class CoronaAPI final : public fl::traits::pin<CoronaAPI>
  {
   public:
    CoronaAPI(boost::asio::io_context& io_context);
    ~CoronaAPI();

    [[nodiscard]] auto dirs() const -> fl::filesystem::application_dirs const&;
    [[nodiscard]] auto dirs_mut() -> fl::filesystem::application_dirs&;

    [[nodiscard]] auto config() const -> CConfig const&;
    [[nodiscard]] auto config_mut() -> CConfig&;

    [[nodiscard]] auto power_switch() const -> modules::CPowerSwitch const&;
    [[nodiscard]] auto power_switch_mut() -> modules::CPowerSwitch&;

    fl::filesystem::application_dirs dirs_;
    CConfig config_;
    fl::box<modules::CPowerSwitch> power_switch_;
  };
} // namespace corona::network