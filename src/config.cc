#include <corona/config.h>

#include <rfl.hpp>
#include <rfl/yaml.hpp>

namespace floppy::serialization
{
  template <> auto serialize<format::yaml>(corona::ConfigData const& value) -> std::string try {
    return rfl::yaml::write(value);
  } catch(std::exception const& e) {
    throw serialization_error(format::yaml, e.what());
  }

  template <> auto deserialize<format::yaml>(std::string const& value) -> corona::ConfigData try {
    return rfl::yaml::read<corona::ConfigData>(value).value();
  } catch(std::exception const& e) {
    throw serialization_error(format::yaml, e.what());
  }
} // namespace floppy::serialization