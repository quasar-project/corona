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
      "{}: {{\n"
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
    return from_exif_data(image_file.filename().string(), exif_data, false, options);
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
    auto self = Metadata();
    self.image_name_ = filename.value_or("raw data");
    self.anchor_point_ = {
      .latitude = *std::bit_cast<f64 const*>(exif_data.data()),
      .longitude = *std::bit_cast<f64 const*>(exif_data.data() + sizeof(f64)),
      .altitude = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 10)
    };
    self.anchor_point_.latitude = *std::bit_cast<f64 const*>(exif_data.data());
    self.anchor_point_.longitude = *std::bit_cast<f64 const*>(exif_data.data() + sizeof(f64));
    self.resolution_ = {
      *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2),
      *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32))
    };
    self.near_edge_ = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 2);
    self.frame_offset_ = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 3);
    self.azimuth_ = fl::math::angle<f32>::from_degrees(*std::bit_cast<f32 const*>(
      exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 4)
    );
    self.drift_angle_ = fl::math::angle<f32>::from_degrees(*std::bit_cast<f32 const*>(
      exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 5)
    );
    self.size_ = {
      *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 6),
      *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 7)
    };
    self.arc_divergence_ = fl::math::angle<f32>::from_degrees(*std::bit_cast<f32 const*>(
      exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 8)
    );
    self.velocity_ = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 9);
    self.frequency_interpolation_coefficient_ = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 11);
    self.time_shift_ = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 12);
    self.time_duration_ = *std::bit_cast<f32 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 13);
    self.sar_mode_ = *std::bit_cast<Mode const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 16);
    self.image_type_ = *std::bit_cast<ImageType const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 16 + sizeof(char));
    //auto const crc = *std::bit_cast<u16 const*>(exif_data.data() + sizeof(f64) * 2 + sizeof(f32) * 18 + sizeof(char) * 2);
    //self.crc_valid_ =
    // todo: crc
    fl::log::debug("successfully parsed exif data from {}", self.name());
    // todo: fix nans
    // https://github.com/quasar-project/deko/blob/main/src/decoder/jpeg_decoder.rs
    // http://uav.radar-mms.com/gitlab/developers/rls/quasar/-/blob/qt5/src/c++/processing/imageprocessing.c++?ref_type=heads
    return self;
  }
} // namespace corona::image