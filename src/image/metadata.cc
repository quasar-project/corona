#include <corona/image/metadata.h>

#include <bit>
#include <fstream>
#include <magic_enum/magic_enum.hpp>
#include <floppy/logging.h>
#include <image/metadata/exif.hh>

namespace llog = floppy::log;
namespace me = magic_enum;

namespace corona::image
{
  auto Metadata::to_string() const -> std::string {
    return fmt::format(
      "\'{}\': {{\n"
      "  anchor: [{}°, {}°, {}],\n"
      "  resolution: {} m/px,\n"
      "  near edge: {} m,\n"
      "  frame offset: {},\n"
      "  azimuth: {}N,\n"
      "  drift angle: {},\n"
      "  size: {} m,\n"
      "  arc divergence: {},\n"
      "  velocity: {} m/s,\n"
      "  fic: {},\n"
      "  time shift: {} s,\n"
      "  time duration: {} s,\n"
      "  sar mode: {},\n"
      "  image type: {},\n"
      "  valid: {}\n"
      "}}",
      this->name(),
      this->anchor_point().latitude,
      this->anchor_point().longitude,
      this->anchor_point().altitude,
      this->resolution(),
      this->near_edge(),
      this->frame_offset(),
      this->azimuth(),
      this->drift_angle(),
      this->size(),
      this->arc_divergence(),
      this->velocity(),
      this->frequency_interpolation_coefficient(),
      this->time_shift(),
      this->time_duration(),
      string(me::enum_name(this->sar_mode())),
      string(me::enum_name(this->image_type())),
      this->crc_valid_ // todo
    );
  }

  auto Metadata::from_exif_file(
    fs::path const& image_file,
    ExifDecodeOptions const& options
  ) noexcept(false) -> Metadata {
    auto ifs = std::ifstream(image_file, std::ios::binary);
    if(not ifs)
      throw decoding_error(image_file, "failed to open file");
    // read first 1024 bytes
    auto exif_data = std::vector<byte>(options.max_metadata_size);
    ifs.read(reinterpret_cast<char*>(exif_data.data()), static_cast<std::streamsize>(exif_data.size())); // NOLINT(*-pro-type-reinterpret-cast)
    if(static_cast<usize>(ifs.gcount()) < exif_data.size())
      throw decoding_error(image_file, fmt::format("exif data too small: {} bytes of at least {} bytes",
        ifs.gcount(),
        exif_data.size())
      );
    return from_exif_data(image_file.stem().string(), exif_data, false, options);
  }

  auto Metadata::from_exif_data(
    option<string_view> filename,
    span<byte const> exif_data,
    bool const relative,
    ExifDecodeOptions const& options
  ) noexcept(false) -> Metadata {
    llog::trace("parsing exif data for {}", filename.value_or("raw data"));
    if(not relative)
      exif_data = exif_data.subspan(options.header_offset);
    auto const header = metadata::ExifHeader::from_bytes(exif_data);
    if(header.marker != options.metadata_marker)
      throw decoding_error(filename.value_or("raw data"), fmt::format("invalid exif marker: 0x{:04X}, expected 0x{:04X}",
        header.marker,
        options.metadata_marker
      ));
    exif_data = exif_data.subspan(sizeof(metadata::ExifHeader));
    if(exif_data.size() < header.size)
      throw decoding_error(filename.value_or("raw data"), fmt::format("exif data too small: {} bytes of at least {} bytes",
        exif_data.size(),
        header.size
      ));
    auto const raw = metadata::ExifMetadata::from_bytes(exif_data);
    auto strip_nan = [](f32 const val) -> f32 {
      if(std::isnan(val) or std::isinf(val))
        return 0.0F;
      return val;
    };
    auto self = Metadata();
    self.image_name_ = filename.value_or("raw data");
    self.anchor_point_ = {
      .latitude = raw.latitude,
      .longitude = raw.longitude,
      .altitude = raw.altitude
    };
    self.resolution_ = { strip_nan(raw.dx), strip_nan(raw.dy) };
    self.near_edge_ = strip_nan(raw.x0);
    self.frame_offset_ = strip_nan(raw.y0);
    self.azimuth_ = fl::math::angle<f32>::from_degrees(strip_nan(raw.azimuth));
    self.drift_angle_ = fl::math::angle<f32>::from_degrees(strip_nan(raw.drift_angle));
    self.size_ = { strip_nan(raw.lx), strip_nan(raw.ly) };
    self.arc_divergence_ = fl::math::angle<f32>::from_degrees(strip_nan(raw.div));
    self.velocity_ = strip_nan(raw.velocity / 3.6F);
    self.frequency_interpolation_coefficient_ = strip_nan(raw.fic);
    self.time_shift_ = strip_nan(raw.time_offset);
    self.time_duration_ = strip_nan(raw.time_duration);
    self.sar_mode_ = static_cast<Mode>(raw.mode);
    self.image_type_ = static_cast<ImageType>(raw.image_type);
    auto const actual_crc = raw.checksum();
    if(actual_crc != raw.crc)
      llog::warn("exif data checksum mismatch: expected 0x{:04X}, actual 0x{:04X}", actual_crc, raw.crc);
    self.crc_valid_ = raw.crc == actual_crc;
    llog::debug("successfully parsed exif data from {}", self.name());
    return self;
  }
} // namespace corona::image