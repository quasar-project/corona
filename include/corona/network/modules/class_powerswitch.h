#pragma once

#include <span>
#include <chrono>
#include <vector>
#include <functional>
#include <corona/detail/export.h>
#include <corona/config.h>

namespace boost::asio { class io_context; } // namespace boost::asio
namespace corona::network::modules
{
  class CPowerSwitch
  {
   public:
    struct ChannelData
    {
      f32 voltage;  ///< Напряжение в вольтах.
      f32 current;  ///< Ток в миллиамперах.
      bool status;  ///< Состояние канала (вкл/выкл).
    };

    explicit CPowerSwitch(
      boost::asio::io_context& io_context,
      CConfig const& config,
      std::function<void()> on_channel_change,
      std::chrono::steady_clock::duration request_interval
    );
    virtual ~CPowerSwitch();

    [[nodiscard]] auto channels() const -> std::vector<ChannelData> const&;
    [[nodiscard]] auto config() const -> CConfig const&;

    auto toggle(usize channel) -> void;
    auto stop() -> void;
    auto restart(std::chrono::steady_clock::duration request_interval) -> void;

   private:
    auto configure(CConfig const& config,std::chrono::steady_clock::duration request_interval) -> void;
    auto request() -> void;
    auto read() -> void;
    auto write(std::span<std::byte const> data) -> void;

    struct impl;
    fl::traits::pimpl<impl> impl_;
  };
} // namespace corona::network::modules