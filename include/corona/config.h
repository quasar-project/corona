#pragma once

#include <vector>
#include <floppy/configuration.h>
#include <corona/detail/export.h>

namespace corona
{
  struct ConfigData
  {
    struct PowerSwitch
    {
      std::vector<string> channel_names = { "АПД", "РЛМ", "ЭВМ", "УМ", "К5", "КОМ", "НАВ" };

      struct Address
      {
        string ip = "192.168.1.50";
        u16 port = 44000;
      } address;

      struct Gauges
      {
        i32 cell_count = 5;
        f32 cell_min_voltage_volts = 3.2F;
        f32 cell_max_voltage_volts = 4.3F;
        f32 max_current_amperes = 7.0F;
      } gauges;

      std::vector<std::pair<f32, f32>> channel_voltages = {
        { 10.0F, 20.0F },
        { 10.0F, 20.0F },
        { 10.0F, 20.0F },
        { 10.0F, 20.0F },
        { 10.0F, 20.0F },
        { 0.0F, 10.0F },
        { 0.0F, 10.0F }
      };
    } power_switch;
  };
} // namespace corona

namespace floppy::serialization
{
  template <> auto serialize<format::yaml>(corona::ConfigData const& value) -> std::string;
  template <> auto deserialize<format::yaml>(std::string const& value) -> corona::ConfigData;
} // namespace floppy::serialization

namespace corona
{
  using CConfig = fl::configuration_file<fl::serialization::format::yaml, ConfigData>;
} // namespace corona