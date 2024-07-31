#include <corona-standalone/map/class_map_memento.hh>

#include <rfl.hpp>
#include <rfl/yaml.hpp>

namespace floppy::serialization
{
  template <> auto serialize<format::yaml>(corona::standalone::map::MapStateData const& value) -> std::string try {
    return rfl::yaml::write(value);
  } catch(std::exception const& e) {
    throw serialization_error(format::yaml, e.what());
  }

  template <> auto deserialize<format::yaml>(std::string const& value) -> corona::standalone::map::MapStateData try {
    return rfl::yaml::read<corona::standalone::map::MapStateData>(value).value();
  } catch(std::exception const& e) {
    throw serialization_error(format::yaml, e.what());
  }
} // namespace floppy::serialization