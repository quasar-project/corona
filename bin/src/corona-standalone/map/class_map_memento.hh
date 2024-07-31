#pragma once

#include <vector>
#include <floppy/configuration.h>
#include <corona-standalone/definitions.hh>

namespace corona::standalone::map
{
  struct MapStateData
  {
    struct Pan
    {
      f64 latitude = 39.0;
      f64 longitude = 139.0;
    } pan;
    f64 zoom = 6.0;
  };
} // namespace corona::standalone::map

namespace floppy::serialization
{
  template <> auto serialize<format::yaml>(corona::standalone::map::MapStateData const& value) -> std::string;
  template <> auto deserialize<format::yaml>(std::string const& value) -> corona::standalone::map::MapStateData;
} // namespace floppy::serialization

namespace corona::standalone::map
{
  using CMapStateMemento = fl::configuration_file<fl::serialization::format::yaml, MapStateData>;
} // namespace corona::standalone::map