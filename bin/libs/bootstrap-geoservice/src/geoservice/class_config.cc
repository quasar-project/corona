#include <geoservice/class_config.hh>

#include <private/qgeomaptype_p.h>
#include <private/qgeocameracapabilities_p.h>
#include <rfl.hpp>
#include <rfl/yaml.hpp>
#include <floppy/logging.h>

namespace llog = floppy::log;
namespace
{
  using namespace std::literals;
  auto constexpr default_cfg_filename = "qnx_geoservice_conf.yml"sv;

  template <typename To, typename From>
  [[nodiscard]] [[maybe_unused]] auto as(From const& value) -> To = delete;

  template <>
  [[nodiscard]] [[maybe_unused]] auto as(corona::bootstrap::geoservice::ConfigData::MapStyle const& value) -> ::QGeoMapType::MapStyle {
    return static_cast<::QGeoMapType::MapStyle>(value);
  }
} // namespace

namespace corona::bootstrap::geoservice
{
  CConfig::CConfig(std::filesystem::path folder)
    : folder_{std::move(folder)}
    , config_{this->folder_ / ::default_cfg_filename, fl::saving_policy::autosave} {
    try {
      llog::trace("geoservice::CConfig: loading config from {}", this->config_.path().string());
      this->config_.load();
    } catch(std::exception const& e) {
      llog::error("geoservice::CConfig: {}", e.what());
    }
  }
  auto CConfig::map_type(usize index) const -> ::QGeoMapType {
    if(index >= this->config_.values().views.size()) {
      llog::error("geoservice::CConfig: index out of bounds");
      return {};
    }

    auto camera_capabilities = ::QGeoCameraCapabilities();
    camera_capabilities.setMinimumZoomLevel((*this)[index].zoom_range.first);
    camera_capabilities.setMaximumZoomLevel((*this)[index].zoom_range.second);
    camera_capabilities.setSupportsBearing((*this)[index].supports_bearing);
    camera_capabilities.setSupportsTilting((*this)[index].supports_tilting);
    camera_capabilities.setSupportsRolling((*this)[index].supports_rolling);
    camera_capabilities.setOverzoomEnabled(false);
    return {
      ::as<::QGeoMapType::MapStyle>((*this)[index].style),
      ::QString::fromStdString((*this)[index].name),
      ::QString::fromStdString((*this)[index].description),
      false,
      false,
      static_cast<int>((*this)[index].id),
      "cgs",
      camera_capabilities,
      ::QVariantMap()
    };
  }

  auto CConfig::as_qlist() const noexcept -> QList<::QGeoMapType> {
    auto res = ::QList<::QGeoMapType>();
    if(not (*this))
      return res;
    for(auto i = 0ZU; i < this->size(); ++i)
      res.append(this->map_type(i));
    return res;
  }
} // namespace corona::bootstrap::geoservice


namespace floppy::serialization
{
  template <> auto serialize<format::yaml>(corona::bootstrap::geoservice::ConfigData const& value) -> std::string try {
    return rfl::yaml::write(value);
  } catch(std::exception const& e) {
    throw serialization_error(format::yaml, e.what());
  }

  template <> auto deserialize<format::yaml>(std::string const& value) -> corona::bootstrap::geoservice::ConfigData try {
    return rfl::yaml::read<corona::bootstrap::geoservice::ConfigData>(value).value();
  } catch(std::exception const& e) {
    throw serialization_error(format::yaml, e.what());
  }
} // namespace floppy::serialization